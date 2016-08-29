use sfml::system::Vector2f;

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

pub trait Orderable {
    fn order(&mut self, order: Order);
}

pub trait Updatable {
    fn update(&mut self, dt: f32);
}

pub trait Animatable {
    fn animate(&mut self);
}
