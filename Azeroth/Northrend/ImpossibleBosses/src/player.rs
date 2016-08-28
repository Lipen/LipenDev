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
    pub clock: Clock,
    time: f32,
    frame: i32,
    animrow: i32,
    // TODO: `stats` field, containing player`s characteristics
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
        match self.order {
            // First, update movement and animation
            // secondary, update texture rect
            Order::Stop => {
                //
            }
            Order::Move { x, y } => {
                let dtarget = Vector2f::new(x, y) - self.shape.get_position();
                let dpos = self.velocity * dt;

                if dpos.len() < dtarget.len() {
                    self.shape.move_(&dpos);

                    self.time += self.clock.restart().as_seconds();
                    let frametime: f32 = 0.2;
                    let maxframes = 2;
                    if self.time > frametime {
                        self.time -= frametime;
                        self.frame += 1;
                        if self.frame > maxframes {
                            self.frame = 0;
                        }
                    }

                    self.rect = IntRect::new(self.size.x as i32 * self.frame,
                                             self.size.y as i32 * self.animrow,
                                             self.size.x as i32,
                                             self.size.y as i32);
                } else {
                    println!("STOPPING!");
                    self.order(Order::Stop);

                    self.shape.set_position2f(x, y);

                    self.rect = IntRect::new(self.size.x as i32,
                                             0,
                                             self.size.x as i32,
                                             self.size.y as i32);
                }
            }
        };

        // self.shape.set_texture_rect(&match self.order {
        //     Order::Stop => IntRect::new(32, 0, 32, 32),
        //     Order::Move { .. } => IntRect::new(32 * self.frame, 32 * self.animrow, 32, 32),
        // });
        self.shape.set_texture_rect(&self.rect);
    }
}

impl<'a> Orderable for Player<'a> {
    fn order(&mut self, order: Order) {
        self.order = order;

        match order {
            Order::Stop => {}
            Order::Move { x, y } => {
                let dpos = Vector2f::new(x, y) - self.shape.get_position();
                let unit = dpos / dpos.len();
                self.velocity = 200. * unit;

                self.clock.restart();
                self.time = 0.;
                self.frame = 0;
                // TODO: choose animrow according to movement direction
                self.animrow = 0;
            }
        }
    }
}

impl<'a> Drawable for Player<'a> {
    fn draw<RT: RenderTarget>(&self, target: &mut RT, rs: &mut RenderStates) {
        target.draw_with_renderstates(&self.shape, rs);
    }
}
