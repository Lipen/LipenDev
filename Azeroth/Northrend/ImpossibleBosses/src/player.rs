use std::collections::HashMap;

use sfml::graphics::{Color, Drawable, FloatRect, IntRect, RectangleShape, RenderStates, RenderTarget, Shape, Texture, Transform, Transformable};
use sfml::system::Vector2f;

use animation::{Animation, AnimationIdentifier};
use bar::HealthBar;
use stats::Stats;
use util_traits::*;


// TODO: animation_map - maybe store Animation in a Box?
// TODO: make animation_map private, add add_animation_frame method
pub struct Player<'a> {
    pub speed: f32,
    pub size: Vector2f,
    shape: RectangleShape<'a>,
    pub order: Order,
    animation_map: HashMap<AnimationIdentifier, Animation>,
    pub animation_cur: AnimationIdentifier,
    pub stats: Stats,
    pub healthbar: HealthBar<'a>,
}

impl<'a> Player<'a> {
    pub fn new(position: Vector2f, speed: f32, size: Vector2f) -> Self {
        let mut shape = RectangleShape::new().unwrap();
        shape.set_position(&position);
        shape.set_size(&size);
        shape.set_origin(&(size / 2.));

        Player {
            speed: speed,
            size: size,
            shape: shape,
            order: Order::Stop,
            animation_map: HashMap::new(),
            animation_cur: AnimationIdentifier::Stay,
            stats: Stats::new(100., 100.),
            healthbar: HealthBar::new(position + Vector2f::new(0., 12.), Vector2f::new(64., 8.)),
        }
    }
}

impl<'a> HasSize for Player<'a> {
    fn get_size(&self) -> &Vector2f {
        &self.size
    }
}

impl<'a> HasShape<'a> for Player<'a> {
    type Output = RectangleShape<'a>;

    fn get_shape(&self) -> &Self::Output {
        &self.shape
    }

    fn get_shape_mut(&mut self) -> &mut Self::Output {
        &mut self.shape
    }
}

impl<'a> HasAnimation for Player<'a> {
    fn get_animation(&self) -> &Animation {
        self.animation_map.get(&self.animation_cur).unwrap()
    }

    fn get_animation_mut(&mut self) -> &mut Animation {
        self.animation_map.get_mut(&self.animation_cur).unwrap()
    }
}

impl<'a> HasAnimationMap for Player<'a> {
    fn get_animationmap(&self) -> &HashMap<AnimationIdentifier, Animation> {
        &self.animation_map
    }
    fn get_animationmap_mut(&mut self) -> &mut HashMap<AnimationIdentifier, Animation> {
        &mut self.animation_map
    }
}

impl<'a> Updatable for Player<'a> {
    fn update(&mut self, dt: f32) {
        let animation_identifier = match self.order {
            Order::Stop => AnimationIdentifier::Stay,
            Order::Move { x, y } => {
                let dtarget = Vector2f::new(x, y) - self.get_position();
                let unit = dtarget / dtarget.len();
                let velocity = self.speed * unit;
                let dpos = velocity * dt;

                if dpos.len() < dtarget.len() {
                    self.move_(&dpos);
                } else {
                    self.set_position2f(x, y);
                    self.order(Order::Stop);
                }

                // TODO: Make choice more robust - what if some animations are absent? They should be replaced with existing. But now it just panics on `unwrapping None`!
                let angle = (dtarget.y).atan2(dtarget.x);
                if 1.22 < angle && angle < 1.92 {
                    // ~70-110 degrees from +x to +y axis => move down
                    AnimationIdentifier::MoveDown
                } else if -1.22 <= angle && angle <= 1.22 {
                    // from -70 to +70 degrees => move right
                    AnimationIdentifier::MoveRight
                } else if -1.92 < angle && angle < -1.22 {
                    // from -70 to -110 degree => move up
                    AnimationIdentifier::MoveUp
                } else {
                    AnimationIdentifier::MoveLeft
                }
            }
        };

        // Reset new animation (only if new (== changed))
        if self.animation_cur != animation_identifier {
            self.animation_map.get_mut(&animation_identifier).unwrap().reset();
            self.animation_cur = animation_identifier;
        }

        self.animate();

        self.healthbar.set_rate(self.stats.health / self.stats.maxhealth);
    }
}

impl<'a> Orderable for Player<'a> {
    fn order(&mut self, order: Order) {
        match order {
            Order::Stop => {
                self.order = order;
            }
            Order::Move { x, y } => {
                let dtarget = Vector2f::new(x, y) - self.shape.get_position();
                // Dead-zone of 1 pixel
                if dtarget.len() >= 1. {
                    self.order = order;
                }
            }
        }
    }
}

impl<'a> Animatable<'a> for Player<'a> {}

impl<'a> Drawable for Player<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        // target.draw_with_renderstates(&self.shape, rs);
        self.shape.draw(target, rs);
        self.healthbar.draw(target, rs);
    }
}

impl<'a> Transformable for Player<'a> {
    fn set_position(&mut self, position: &Vector2f) {
        self.shape.set_position(&position);
        self.healthbar.set_position2f(position.x - self.size.x / 2., position.y + 12.);
    }
    fn set_position2f(&mut self, x: f32, y: f32) {
        self.shape.set_position2f(x, y);
        self.healthbar.set_position2f(x - self.size.x / 2., y + 12.);
    }
    fn set_rotation(&mut self, angle: f32) {
        self.shape.set_rotation(angle);
    }
    fn set_scale(&mut self, scale: &Vector2f) {
        self.shape.set_scale(&scale);
        // FIXME: Should healthbar also be scaled?
        self.healthbar.set_scale(&scale);
    }
    fn set_scale2f(&mut self, scale_x: f32, scale_y: f32) {
        self.shape.set_scale2f(scale_x, scale_y);
        // FIXME: Should healthbar also be scaled?
        self.healthbar.set_scale2f(scale_x, scale_y);
    }
    fn set_origin(&mut self, origin: &Vector2f) {
        self.shape.set_origin(&origin);
    }
    fn set_origin2f(&mut self, x: f32, y: f32) {
        self.shape.set_origin2f(x, y);
    }
    fn get_position(&self) -> Vector2f {
        self.shape.get_position()
    }
    fn get_rotation(&self) -> f32 {
        self.shape.get_rotation()
    }
    fn get_scale(&self) -> Vector2f {
        self.shape.get_scale()
    }
    fn get_origin(&self) -> Vector2f {
        self.shape.get_origin()
    }
    fn move_(&mut self, offset: &Vector2f) {
        self.shape.move_(&offset);
        self.healthbar.move_(&offset);
    }
    fn move2f(&mut self, offset_x: f32, offset_y: f32) {
        self.shape.move2f(offset_x, offset_y);
        self.healthbar.move2f(offset_x, offset_y);
    }
    fn rotate(&mut self, angle: f32) {
        self.shape.rotate(angle);
    }
    fn scale(&mut self, factors: &Vector2f) {
        self.shape.scale(&factors);
        // FIXME: Should healthbar also be scaled?
        self.healthbar.scale(&factors);
    }
    fn scale2f(&mut self, factor_x: f32, factor_y: f32) {
        self.shape.scale2f(factor_x, factor_y);
        // FIXME: Should healthbar also be scaled?
        self.healthbar.scale2f(factor_x, factor_y);
    }
    fn get_transform(&self) -> Transform {
        self.shape.get_transform()
    }
    fn get_inverse_transform(&self) -> Transform {
        self.shape.get_inverse_transform()
    }
}

impl<'a> Shape<'a> for Player<'a> {
    fn set_texture(&mut self, texture: &'a Texture, reset_rect: bool) {
        self.shape.set_texture(&texture, reset_rect);
    }
    fn disable_texture(&mut self) {
        self.shape.disable_texture();
    }
    fn set_texture_rect(&mut self, rect: &IntRect) {
        self.shape.set_texture_rect(&rect);
    }
    fn set_fill_color(&mut self, color: &Color) {
        self.shape.set_fill_color(&color);
    }
    fn set_outline_color(&mut self, color: &Color) {
        self.shape.set_outline_color(&color);
    }
    fn set_outline_thickness(&mut self, thickness: f32) {
        self.shape.set_outline_thickness(thickness);
    }
    fn get_texture(&self) -> Option<&'a Texture> {
        self.shape.get_texture()
    }
    fn get_texture_rect(&self) -> IntRect {
        self.shape.get_texture_rect()
    }
    fn get_fill_color(&self) -> Color {
        self.shape.get_fill_color()
    }
    fn get_outline_color(&self) -> Color {
        self.shape.get_outline_color()
    }
    fn get_outline_thickness(&self) -> f32 {
        self.shape.get_outline_thickness()
    }
    fn get_point_count(&self) -> u32 {
        self.shape.get_point_count()
    }
    fn get_point(&self, index: u32) -> Vector2f {
        self.shape.get_point(index)
    }
    fn get_local_bounds(&self) -> FloatRect {
        self.shape.get_local_bounds()
    }
    fn get_global_bounds(&self) -> FloatRect {
        self.shape.get_global_bounds()
    }
}
