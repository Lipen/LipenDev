use std::collections::HashMap;

use sfml::graphics::{IntRect, Shape};
use sfml::system::Vector2f;

use animation::{Animation, AnimationIdentifier};


pub trait VectorExtension {
    fn rotate(&mut self, angle: f32);
    fn len(&self) -> f32;
}

impl VectorExtension for Vector2f {
    fn rotate(&mut self, angle: f32) {
        let angle_sin = angle.sin();
        let angle_cos = angle.cos();
        self.x = self.x * angle_cos - self.y * angle_sin;
        self.y = self.y * angle_cos + self.x * angle_sin;
    }

    fn len(&self) -> f32 {
        (self.x * self.x + self.y * self.y).sqrt() as f32
    }
}

#[derive(PartialEq, Clone, Copy)]
pub enum Order {
    Stop,
    Move {
        x: f32,
        y: f32,
    },
}

pub trait HasSize {
    fn get_size(&self) -> &Vector2f;
}

pub trait HasShape<'a> {
    type Output: Shape<'a>;
    fn get_shape(&self) -> &Self::Output;
    fn get_shape_mut(&mut self) -> &mut Self::Output;
}

pub trait HasAnimation {
    fn get_animation(&self) -> &Animation;
    fn get_animation_mut(&mut self) -> &mut Animation;
}

pub trait HasAnimationMap {
    fn get_animationmap(&self) -> &HashMap<AnimationIdentifier, Animation>;
    fn get_animationmap_mut(&mut self) -> &mut HashMap<AnimationIdentifier, Animation>;
}

pub trait Orderable {
    fn order(&mut self, order: Order);
}

pub trait Updatable {
    fn update(&mut self, dt: f32);
}

pub trait Animatable<'a>
    : HasAnimation + HasAnimationMap + HasShape<'a> + HasSize {
    fn add_animation_frame(&mut self, identifier: AnimationIdentifier, animrow: i32, frame_max: i32, frame_time: f32) {
        self.get_animationmap_mut().insert(identifier, Animation::new(animrow, frame_max, frame_time));
    }

    fn animate(&mut self) {
        self.get_animation_mut().update();
        let &Vector2f { x: w, y: h } = self.get_size();
        let &Animation { frame_cur, animrow, .. } = self.get_animation();
        let rect = IntRect::new(w as i32 * frame_cur, h as i32 * animrow, w as i32, h as i32);
        self.get_shape_mut().set_texture_rect(&rect);
    }
}
