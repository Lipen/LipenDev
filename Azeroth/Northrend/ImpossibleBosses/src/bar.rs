use sfml::graphics::{Color, Drawable, RectangleShape, RenderStates, RenderTarget, Shape, Transform, Transformable};
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
        shape.set_position(&Vector2f::new(position.x - size.x / 2., position.y));
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
        let pos = Vector2f::new(position.x - self.size.x / 2., position.y);
        self.shape_back.set_position(&pos);
        self.shape_front.set_position(&pos);
    }

    pub fn set_percent(&mut self, percent: f32) {
        let newsize = Vector2f::new(self.size.x * percent, self.size.y);
        self.shape_front.set_size(&newsize);
    }
}

impl<'a> Updatable for HealthBar<'a> {
    fn update(&mut self, dt: f32) {
        //
    }
}

impl<'a> Drawable for HealthBar<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        self.shape_back.draw(target, rs);
        self.shape_front.draw(target, rs);
    }
}


impl<'a> Transformable for HealthBar<'a> {
    fn set_position(&mut self, position: &Vector2f) {
        self.shape_back.set_position(&position);
        self.shape_front.set_position(&position);
    }
    fn set_position2f(&mut self, x: f32, y: f32) {
        self.shape_back.set_position2f(x, y);
        self.shape_front.set_position2f(x, y);
    }
    fn set_rotation(&mut self, angle: f32) {
        self.shape_back.set_rotation(angle);
        self.shape_front.set_rotation(angle);
    }
    fn set_scale(&mut self, scale: &Vector2f) {
        self.shape_back.set_scale(&scale);
        self.shape_front.set_scale(&scale);
    }
    fn set_scale2f(&mut self, scale_x: f32, scale_y: f32) {
        self.shape_back.set_scale2f(scale_x, scale_y);
        self.shape_front.set_scale2f(scale_x, scale_y);
    }
    fn set_origin(&mut self, origin: &Vector2f) {
        self.shape_back.set_origin(&origin);
        self.shape_front.set_origin(&origin);
    }
    fn set_origin2f(&mut self, x: f32, y: f32) {
        self.shape_back.set_origin2f(x, y);
        self.shape_front.set_origin2f(x, y);
    }
    fn get_position(&self) -> Vector2f {
        self.shape_back.get_position()
    }
    fn get_rotation(&self) -> f32 {
        self.shape_back.get_rotation()
    }
    fn get_scale(&self) -> Vector2f {
        self.shape_back.get_scale()
    }
    fn get_origin(&self) -> Vector2f {
        self.shape_back.get_origin()
    }
    fn move_(&mut self, offset: &Vector2f) {
        self.shape_back.move_(&offset);
        self.shape_front.move_(&offset);
    }
    fn move2f(&mut self, offset_x: f32, offset_y: f32) {
        self.shape_back.move2f(offset_x, offset_y);
        self.shape_front.move2f(offset_x, offset_y);
    }
    fn rotate(&mut self, angle: f32) {
        self.shape_back.rotate(angle);
        self.shape_front.rotate(angle);
    }
    fn scale(&mut self, factors: &Vector2f) {
        self.shape_back.scale(&factors);
        self.shape_front.scale(&factors);
    }
    fn scale2f(&mut self, factor_x: f32, factor_y: f32) {
        self.shape_back.scale2f(factor_x, factor_y);
        self.shape_front.scale2f(factor_x, factor_y);
    }
    fn get_transform(&self) -> Transform {
        self.shape_back.get_transform()
    }
    fn get_inverse_transform(&self) -> Transform {
        self.shape_back.get_inverse_transform()
    }
}
