extern crate sfml;
pub mod player;
pub mod particle;
pub mod util_traits;

use sfml::graphics::{Color, RenderWindow, RenderTarget};
use sfml::window::{Event, ContextSettings, Key, VideoMode, MouseButton};
use sfml::system::{Clock, Vector2f};
use std::env;
use player::Player;
use particle::Particle;
#[allow(unused_imports)]
use util_traits::*;


fn main() {
    // Set up aa level
    let mut aa_level = 0;

    if let Some(arg) = env::args().nth(1) {
        if let Ok(arg_as_num) = arg.parse::<u32>() {
            println!("Using {}xAA", arg_as_num);
            aa_level = arg_as_num;
        }
    }

    // Define some constants
    let game_width: u32 = 800;
    let game_height: u32 = 600;

    // Create the window of the application
    let mut window = RenderWindow::new(VideoMode::new_init(game_width, game_height, 32),
                                       "Impossible bosses",
                                       Default::default(),
                                       ContextSettings::new().antialiasing(aa_level))
        .unwrap();
    // window.set_vertical_sync_enabled(true);

    // Create player
    let mut player = Player::new(Vector2f::new(300., 400.), 200., Vector2f::new(50., 80.));
    // Set up animations
    // player.animations.emplace_animation("walking",
    //                                     "assets/spritesheet.png",
    //                                     vec![AnimationFrame::new(32, 64, 32, 32),
    //                                          AnimationFrame::new(64, 64, 32, 32),
    //                                          AnimationFrame::new(32, 64, 32, 32),
    //                                          AnimationFrame::new(0, 64, 32, 32)]);

    // Initialize particles vector
    let mut particles: Vec<Particle> = vec![];

    // Create clock
    let mut clock = Clock::new();

    // Main loop
    loop {
        for event in window.events() {
            // TODO: call process_event(event)
            match event {
                Event::Closed => return,
                Event::KeyPressed { code, .. } => {
                    match code {
                        Key::Escape => return,
                        Key::W => {
                            player.moving_up = true;
                        }
                        Key::A => {
                            player.moving_left = true;
                        }
                        Key::S => {
                            player.moving_down = true;
                        }
                        Key::D => {
                            player.moving_right = true;
                        }
                        _ => {}
                    }
                }
                Event::KeyReleased { code, .. } => {
                    match code {
                        Key::W => {
                            player.moving_up = false;
                        }
                        Key::A => {
                            player.moving_left = false;
                        }
                        Key::S => {
                            player.moving_down = false;
                        }
                        Key::D => {
                            player.moving_right = false;
                        }
                        _ => {}
                    }
                }
                Event::MouseButtonPressed { button, x, y } => {
                    if let MouseButton::Left = button {
                        let mut particle = Particle::new(player.position, 10., 3.);
                        let dpos = Vector2f::new(x as f32, y as f32) - particle.position;
                        let unit = dpos / (dpos.x * dpos.x + dpos.y * dpos.y).sqrt();

                        particle.velocity = 200. * unit;
                        particle.acceleration = -30. * unit;

                        particles.push(particle);
                        println!("Particle spawned!");
                    }
                }
                Event::MouseButtonReleased { button, x, y } => {}
                _ => {}
            }
        }

        let time_delta = clock.restart().as_seconds();

        // TODO: call Game.update() or something similar
        player.update(time_delta);

        for mut p in &mut particles {
            p.update(time_delta);
        }

        // Remove expired particles:
        particles.retain(|ref p| p.clock.get_elapsed_time().as_seconds() < p.lifetime);

        // TODO: call Game.draw(&window) or something similar
        window.clear(&Color::new_rgb(200, 200, 200));
        window.draw(&player);

        // p: &Particle
        for p in &particles {
            window.draw(p);
        }

        window.display();
    }
}
