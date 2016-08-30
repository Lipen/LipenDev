use std::collections::HashMap;

use sfml::graphics::{Drawable, IntRect, RectangleShape, RenderStates, RenderTarget, Shape, Transformable};
use sfml::system::Vector2f;

use animation::{Animation, AnimationIdentifier};
use util_traits::*;


// TODO: `stats` field, containing player`s characteristics
// TODO: animation_map - maybe store Animation in a Box?
// TODO: impl Transformable for Player
pub struct Player<'a> {
    pub speed: f32,
    pub velocity: Vector2f,
    pub size: Vector2f,
    pub shape: RectangleShape<'a>,
    pub order: Order,
    pub animation_map: HashMap<AnimationIdentifier, Animation>,
    pub animation_cur: AnimationIdentifier,
}

impl<'a> Player<'a> {
    pub fn new(position: Vector2f, speed: f32, size: Vector2f) -> Self {
        let mut shape = RectangleShape::new().unwrap();
        shape.set_position(&position);
        shape.set_size(&size);
        shape.set_origin(&(size / 2.));

        Player {
            speed: speed,
            velocity: Vector2f::new(0., 0.),
            size: size,
            shape: shape,
            order: Order::Stop,
            animation_map: HashMap::new(),
            animation_cur: AnimationIdentifier::Stay,
        }
    }
}

impl<'a> Updatable for Player<'a> {
    fn update(&mut self, dt: f32) {
        let animation_identifier = match self.order {
            Order::Stop => AnimationIdentifier::Stay,
            Order::Move { x, y } => {
                let dtarget = Vector2f::new(x, y) - self.shape.get_position();
                let unit = dtarget / dtarget.len();
                self.velocity = 200. * unit;
                let dpos = self.velocity * dt;

                if dpos.len() < dtarget.len() {
                    self.shape.move_(&dpos);
                } else {
                    self.shape.set_position2f(x, y);
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

impl<'a> Animatable for Player<'a> {
    fn animate(&mut self) {
        let mut animation = self.animation_map.get_mut(&self.animation_cur).unwrap();

        animation.update();

        let rect = IntRect::new(self.size.x as i32 * animation.frame_cur,
                                self.size.y as i32 * animation.animrow,
                                self.size.x as i32,
                                self.size.y as i32);
        self.shape.set_texture_rect(&rect);
    }
}

impl<'a> Drawable for Player<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
