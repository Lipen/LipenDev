use sfml::graphics::{Color, Drawable, RectangleShape, RenderStates, RenderTarget, Shape, Transformable};
use sfml::system::Vector2f;

use util_traits::*;


pub struct HealthBar<'a> {
    pub size: Vector2f,
    pub shape_back: RectangleShape<'a>,
    pub shape_front: RectangleShape<'a>,
    pub percent: f32,
}

impl<'a> HealthBar<'a> {
    pub fn new(position: Vector2f, size: Vector2f) -> Self {
        let mut shape = RectangleShape::new().unwrap();
        shape.set_position(&Vector2f::new(position.x - size.x/2., position.y));
        shape.set_size(&size);
        shape.set_origin(&Vector2f::new(0., size.y / 2.));

        let mut shape_back = shape.clone();
        shape_back.set_fill_color(&Color::black());

        let mut shape_front = shape.clone();
        shape_front.set_fill_color(&Color::new_rgb(24, 240, 0));

        HealthBar {
            size: size,
            shape_back: shape_back,
            shape_front: shape_front,
            percent: 100.,
        }
    }

    pub fn set_position(&mut self, position: &Vector2f) {
        let pos = Vector2f::new(position.x - self.size.x/2., position.y);
        self.shape_back.set_position(&pos);
        self.shape_front.set_position(&pos);
    }

    pub fn set_percent(&mut self, percent: f32) {
        let newsize = Vector2f::new(self.size.x * percent, self.size.y);
        self.shape_front.set_size(&newsize);
    }
}

impl<'a> Drawable for HealthBar<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        self.shape_back.draw(target, rs);
        self.shape_front.draw(target, rs);
    }
}

impl<'a> Updatable for HealthBar<'a> {
    fn update(&mut self, dt: f32) {
        //
    }
}
