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

impl<'a> Animatable<'a> for Particle<'a> {}

impl<'a> Drawable for Particle<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
