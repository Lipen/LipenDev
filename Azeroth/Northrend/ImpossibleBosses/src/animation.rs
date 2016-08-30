use sfml::system::Clock;


#[derive(PartialEq, Eq, Hash, Clone, Copy)]
pub enum AnimationIdentifier {
    MoveUp,
    MoveLeft,
    MoveDown,
    MoveRight,
    Stay,
    Move,
}

pub struct Animation {
    clock: Clock,
    time: f32,
    pub animrow: i32,
    pub frame_cur: i32,
    pub frame_max: i32,
    pub frame_time: f32,
}

impl Animation {
    pub fn new(animrow: i32, frame_max: i32, frame_time: f32) -> Self {
        Animation {
            clock: Clock::new(),
            time: 0.0,
            animrow: animrow,
            frame_cur: 0,
            frame_max: frame_max,
            frame_time: frame_time,
        }
    }

    pub fn update(&mut self) {
        self.time += self.clock.restart().as_seconds();
        if self.time > self.frame_time {
            self.time -= self.frame_time;
            self.frame_cur += 1;
            if self.frame_cur >= self.frame_max {
                self.frame_cur = 0;
            }
        }
    }

    pub fn reset(&mut self) {
        self.clock.restart();
        self.time = 0.;
        self.frame_cur = 0;
        println!("Resetting!");
    }
}
