use sfml::graphics::{RenderTarget, RenderStates, Drawable, RectangleShape, Transformable, Shape,
                     IntRect};
use sfml::system::{Clock, Vector2f};

use util_traits::*;


pub struct Player<'a> {
    pub speed: f32,
    pub velocity: Vector2f,
    pub size: Vector2f,
    rect: IntRect,
    pub shape: RectangleShape<'a>,
    pub order: Order,
    clock: Clock,
    time: f32,
    frame: i32,
    animrow: i32, // TODO: `stats` field, containing player`s characteristics
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
            rect: IntRect::new(size.x as i32, 0, size.x as i32, size.y as i32),
            order: Order::Stop,
            clock: Clock::new(),
            time: 0.,
            frame: 0,
            animrow: 0,
        }
    }
}

impl<'a> Updatable for Player<'a> {
    fn update(&mut self, dt: f32) {
        // 1: update animation
        // 2: update position
        // 3: move
        // 4: update texture rect

        // TODO: Replace own clock with use or `dt` argument
        self.time += self.clock.restart().as_seconds();
        let maxframes = 16;
        let frametime: f32 = 1.067 / maxframes as f32;
        if self.time > frametime {
            self.time -= frametime;
            self.frame += 1;
            if self.frame >= maxframes {
                self.frame = 0;
            }
        }

        match self.order {
            Order::Stop => {
                // staying...
                self.animrow = 0;
            }
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

                let angle = (dtarget.y).atan2(dtarget.x);
                if 1.22 < angle && angle < 1.92 {
                    // ~70-110 degrees from +x to +y axis => move down
                    self.animrow = 0;
                } else if -1.22 <= angle && angle <= 1.22 {
                    // from -70 to +70 degrees => move right
                    self.animrow = 2;
                } else if -1.92 < angle && angle < -1.22 {
                    // from -70 to -110 degree => move up
                    self.animrow = 3;
                } else {
                    // move left
                    self.animrow = 1;
                }
            }
        };

        self.rect = IntRect::new(self.size.x as i32 * self.frame,
                                 self.size.y as i32 * self.animrow,
                                 self.size.x as i32,
                                 self.size.y as i32);
        self.shape.set_texture_rect(&self.rect);
    }
}

impl<'a> Orderable for Player<'a> {
    fn order(&mut self, order: Order) {
        self.order = order;

        match order {
            Order::Stop => {
                self.clock.restart();
                self.time = 0.;
                self.frame = 0;
            }
            Order::Move { .. } => {
                self.clock.restart();
                self.time = 0.;
                self.frame = 0;
            }
        }
    }
}

impl<'a> Drawable for Player<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
