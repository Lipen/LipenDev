use sfml::graphics::{Color, CircleShape, RenderTarget, RenderStates, Shape, Transformable,
                     Drawable};
use sfml::system::{Clock, Vector2f};

use util_traits::*;


pub struct Particle<'a> {
    pub position: Vector2f,
    pub velocity: Vector2f,
    pub acceleration: Vector2f,
    pub lifetime: f32,
    pub shape: CircleShape<'a>,
    pub clock: Clock,
}

impl<'a> Particle<'a> {
    pub fn new(position: Vector2f, radius: f32, lifetime: f32) -> Self {
        let mut shape = CircleShape::new().unwrap();
        shape.set_position(&position);
        shape.set_radius(radius - 2.);
        shape.set_outline_thickness(2.);
        shape.set_outline_color(&Color::black());
        shape.set_fill_color(&Color::new_rgb(0, 150, 136));
        shape.set_origin(&Vector2f::new(radius / 2., radius / 2.));

        Particle {
            position: position,
            velocity: Vector2f::new(0., 0.),
            acceleration: Vector2f::new(0., 0.),
            lifetime: lifetime,
            shape: shape,
            clock: Clock::new(),
        }
    }
}

impl<'a> Updatable for Particle<'a> {
    fn update(&mut self, dt: f32) {
        self.velocity = self.velocity + self.acceleration * dt;
        self.position = self.position + self.velocity * dt;
        self.shape.set_position(&self.position);
    }
}

impl<'a> Drawable for Particle<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
