use sfml::graphics::{Color, RectangleShape, RenderTarget, RenderStates, Shape, Transformable,
                     Drawable};
use sfml::system::{Clock, Vector2f};
use util_traits::*;


pub struct Player<'a> {
    pub position: Vector2f,
    pub speed: f32,
    pub velocity: Vector2f,
    // TODO: Make Option<RectangleShape>
    // TODO: Replace with generic &Shape
    pub shape: RectangleShape<'a>,
    pub moving_up: bool,
    pub moving_left: bool,
    pub moving_down: bool,
    pub moving_right: bool,
}

impl<'a> Player<'a> {
    pub fn new(position: Vector2f, speed: f32, size: Vector2f) -> Player<'a> {
        let mut shape = RectangleShape::new().unwrap();
        shape.set_position(&position);
        shape.set_size(&(size - 2.));
        shape.set_outline_thickness(2.);
        shape.set_outline_color(&Color::black());
        shape.set_fill_color(&Color::new_rgb(0, 150, 136));
        shape.set_origin(&(size / 2.));

        Player {
            position: position,
            speed: speed,
            velocity: Vector2f::new(0., 0.),
            shape: shape,
            moving_up: false,
            moving_left: false,
            moving_down: false,
            moving_right: false,
        }
    }
}

impl<'a> Updatable for Player<'a> {
    fn update(&mut self, dt: f32) {
        self.velocity.x = (self.moving_right as i8 - self.moving_left as i8) as f32 * self.speed;
        self.velocity.y = (self.moving_down as i8 - self.moving_up as i8) as f32 * self.speed;

        self.position = self.position + self.velocity * dt;  // idk why `+=` doesn't work...
        self.shape.set_position(&self.position);
    }
}

impl<'a> Drawable for Player<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
