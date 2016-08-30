use std::collections::HashMap;

use sfml::graphics::{Drawable, IntRect, RectangleShape, RenderStates, RenderTarget, Shape, Transformable};
use sfml::system::{Clock, Vector2f};

use animation::{Animation, AnimationIdentifier};
use util_traits::*;


pub struct Particle<'a> {
    pub speed: f32,
    pub velocity: Vector2f,
    pub size: Vector2f,
    pub shape: RectangleShape<'a>,
    pub order: Order,
    pub animation_map: HashMap<AnimationIdentifier, Animation>,
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
}

impl<'a> Updatable for Particle<'a> {
    fn update(&mut self, dt: f32) {
        let animation_identifier = match self.order {
            Order::Stop => AnimationIdentifier::Move,
            Order::Move { .. } => {
                let dpos = self.velocity * dt;
                self.shape.move_(&dpos);
                AnimationIdentifier::Move
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
                    self.velocity = 200. * unit;
                }
            }
        }
    }
}

impl<'a> Animatable for Particle<'a> {
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

impl<'a> Drawable for Particle<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
