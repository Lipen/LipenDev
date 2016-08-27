use sfml::system::Vector2f;


pub trait Updatable {
    fn update(&mut self, dt: f32);
}

pub trait VectorRotator {
    fn rotate(&mut self, angle: f32);
}

impl VectorRotator for Vector2f {
    fn rotate(&mut self, angle: f32) {
        let angle_sin = angle.sin();
        let angle_cos = angle.cos();
        self.x = self.x * angle_cos - self.y * angle_sin;
        self.y = self.y * angle_cos + self.x * angle_sin;
    }
}
