use std::collections::HashMap;

use sfml::graphics::{Color, Drawable, FloatRect, IntRect, RectangleShape, RenderStates, RenderTarget, Shape, Texture, Transform, Transformable};
use sfml::system::{Clock, Vector2f};

use animation::{Animation, AnimationIdentifier};
use util_traits::*;


pub struct Particle<'a> {
    pub speed: f32,
    pub velocity: Vector2f,
    size: Vector2f,
    shape: RectangleShape<'a>,
    pub order: Order,
    animation_map: HashMap<AnimationIdentifier, Animation>,
    pub animation_cur: AnimationIdentifier,
    pub lifetime: f32,
    pub lifeclock: Clock,
}

impl<'a> Particle<'a> {
    pub fn new(position: Vector2f, speed: f32, size: Vector2f, lifetime: f32) -> Self {
        let mut shape = RectangleShape::new().unwrap();
        shape.set_position(&position);
        shape.set_size(&size);
        shape.set_origin(&(size / 2.));

        Particle {
            speed: speed,
            velocity: Vector2f::new(0., 0.),
            size: size,
            shape: shape,
            order: Order::Stop,
            animation_map: HashMap::new(),
            animation_cur: AnimationIdentifier::Stay,
            lifetime: lifetime,
            lifeclock: Clock::new(),
        }
    }

    pub fn collide_with(&mut self, mut other: &mut Particle) {
        let dr = self.get_position() - other.get_position();
        let dist_sqr = dr.len_sqr();

        if dist_sqr <= 16f32.powi(2) {
            println!("Collision on dist = {}", dr.len());
            let dv = self.velocity - other.velocity;
            let alpha = -dv.dot(&dr) / dist_sqr;
            let deltav = alpha * dr;
            self.velocity = self.velocity + deltav;
            other.velocity = other.velocity - deltav;
        }
    }
}

impl<'a> HasSize for Particle<'a> {
    fn get_size(&self) -> &Vector2f {
        &self.size
    }
}

impl<'a> HasShape<'a> for Particle<'a> {
    type Output = RectangleShape<'a>;

    fn get_shape(&self) -> &Self::Output {
        &self.shape
    }

    fn get_shape_mut(&mut self) -> &mut Self::Output {
        &mut self.shape
    }
}

impl<'a> HasAnimation for Particle<'a> {
    fn get_animation(&self) -> &Animation {
        self.animation_map.get(&self.animation_cur).unwrap()
    }

    fn get_animation_mut(&mut self) -> &mut Animation {
        self.animation_map.get_mut(&self.animation_cur).unwrap()
    }
}

impl<'a> HasAnimationMap for Particle<'a> {
    fn get_animationmap(&self) -> &HashMap<AnimationIdentifier, Animation> {
        &self.animation_map
    }
    fn get_animationmap_mut(&mut self) -> &mut HashMap<AnimationIdentifier, Animation> {
        &mut self.animation_map
    }
}

impl<'a> Updatable for Particle<'a> {
    fn update(&mut self, dt: f32) {
        let animation_identifier = AnimationIdentifier::Move;

        let dpos = self.velocity * dt;
        self.move_(&dpos);

        let Vector2f{x, y} = self.get_position();
        if x > 800. {
            self.set_position2f(800., y);
            self.velocity.x *= -1.;
        } else if x < 0. {
            self.set_position2f(0., y);
            self.velocity.x *= -1.;
        }

        let Vector2f{x, y} = self.get_position();
        if y > 600. {
            self.set_position2f(x, 600.);
            self.velocity.y *= -1.;
        } else if y < 0. {
            self.set_position2f(x, 0.);
            self.velocity.y *= -1.;
        }

        // Reset new animation (only if new (== changed))
        if self.animation_cur != animation_identifier {
            self.animation_map.get_mut(&animation_identifier).unwrap().reset();
            self.animation_cur = animation_identifier;
        }

        self.animate();
    }
}

impl<'a> Orderable for Particle<'a> {
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
                    let unit = dtarget / dtarget.len();
                    self.velocity = self.speed * unit;
                }
            }
        }
    }
}

impl<'a> Animatable<'a> for Particle<'a> {}

impl<'a> Drawable for Particle<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}

impl<'a> Transformable for Particle<'a> {
    fn set_position(&mut self, position: &Vector2f) {
        self.shape.set_position(&position);
    }
    fn set_position2f(&mut self, x: f32, y: f32) {
        self.shape.set_position2f(x, y);
    }
    fn set_rotation(&mut self, angle: f32) {
        self.shape.set_rotation(angle);
    }
    fn set_scale(&mut self, scale: &Vector2f) {
        self.shape.set_scale(&scale);
    }
    fn set_scale2f(&mut self, scale_x: f32, scale_y: f32) {
        self.shape.set_scale2f(scale_x, scale_y);
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
    }
    fn move2f(&mut self, offset_x: f32, offset_y: f32) {
        self.shape.move2f(offset_x, offset_y);
    }
    fn rotate(&mut self, angle: f32) {
        self.shape.rotate(angle);
    }
    fn scale(&mut self, factors: &Vector2f) {
        self.shape.scale(&factors);
    }
    fn scale2f(&mut self, factor_x: f32, factor_y: f32) {
        self.shape.scale2f(factor_x, factor_y);
    }
    fn get_transform(&self) -> Transform {
        self.shape.get_transform()
    }
    fn get_inverse_transform(&self) -> Transform {
        self.shape.get_inverse_transform()
    }
}

impl<'a> Shape<'a> for Particle<'a> {
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
