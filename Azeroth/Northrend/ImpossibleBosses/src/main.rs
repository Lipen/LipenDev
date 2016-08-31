extern crate sfml;

use std::env;

use sfml::graphics::*;
use sfml::system::*;
use sfml::window::*;

pub mod player;
use player::*;
pub mod particle;
use particle::*;
pub mod resource_manager;
use resource_manager::*;
pub mod animation;
use animation::*;
pub mod bar;
use bar::*;
pub mod stats;
use stats::*;
pub mod util_traits;
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
                                       window_style::CLOSE,
                                       ContextSettings::new().antialiasing(aa_level))
        .unwrap();
    // window.set_vertical_sync_enabled(true);

    // Initialize texture manager and load textures
    let mut texture_manager = TextureManager::new();
    texture_manager.load(TextureIdentifier::Wizard, "assets/wizard.png");
    texture_manager.load(TextureIdentifier::Andromalius, "assets/andromalius.png");
    texture_manager.load(TextureIdentifier::Particle, "assets/particle.png");

    // Create player and setup its animations
    let mut player = Player::new(Vector2f::new(300., 400.), 200., Vector2f::new(64., 64.));
    player.shape.set_texture(texture_manager.get(TextureIdentifier::Wizard), false);
    player.shape.set_origin(&Vector2f::new(32., 61.));
    player.animation_map.insert(AnimationIdentifier::Stay, Animation::new(0, 16, 0.1));
    player.animation_map.insert(AnimationIdentifier::MoveLeft, Animation::new(1, 16, 0.08));
    player.animation_map.insert(AnimationIdentifier::MoveRight, Animation::new(2, 16, 0.08));
    player.animation_map.insert(AnimationIdentifier::MoveUp, Animation::new(3, 16, 0.08));
    player.animation_map.insert(AnimationIdentifier::MoveDown, Animation::new(0, 16, 0.08));

    // Create enemy
    let mut andromalius = Player::new(Vector2f::new(500., 100.), 100., Vector2f::new(57., 88.));
    andromalius.shape.set_texture(texture_manager.get(TextureIdentifier::Andromalius), false);
    andromalius.shape.set_origin(&Vector2f::new(57. / 2., 80.));
    andromalius.animation_map.insert(AnimationIdentifier::Stay, Animation::new(3, 8, 0.2));
    andromalius.animation_map.insert(AnimationIdentifier::MoveLeft, Animation::new(4, 8, 0.15));
    andromalius.animation_map.insert(AnimationIdentifier::MoveRight, Animation::new(0, 8, 0.15));

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
                        Key::Space => {
                            println!("[d] player.shape.get_position():\n\t{:?}",
                                     player.shape.get_position());
                            println!("[d] player.shape.get_texture_rect():\n\t{:?}",
                                     player.shape.get_texture_rect());
                        }
                        _ => {}
                    }
                }
                // Event::KeyReleased { code, .. } => {
                //     match code {
                //         _ => {}
                //     }
                // }
                Event::MouseButtonPressed { button, x, y } => {
                    if let MouseButton::Left = button {
                        let mut particle = Particle::new(player.shape.get_position(),
                                                         300.,
                                                         Vector2f::new(128., 128.),
                                                         3.);
                        particle.shape.set_texture(texture_manager.get(TextureIdentifier::Particle), false);
                        particle.shape.set_scale2f(0.25, 0.25);
                        particle.animation_map.insert(AnimationIdentifier::Move, Animation::new(0, 26, 0.1));
                        particle.order(Order::Move {
                            x: x as f32,
                            y: y as f32,
                        });

                        player.stats.health *= 0.95;

                        particles.push(particle);
                        println!("Particle spawned!");
                    }

                    if let MouseButton::Right = button {
                        if Key::LControl.is_pressed() {
                            andromalius.order(Order::Move {
                                x: x as f32,
                                y: y as f32,
                            })
                        } else {
                            player.order(Order::Move {
                                x: x as f32,
                                y: y as f32,
                            });
                        }
                    }
                }
                // Event::MouseButtonReleased { button, x, y } => {}
                _ => {}
            }
        }

        let time_delta = clock.restart().as_seconds();

        // TODO: call Game.update() or something similar
        player.update(time_delta);
        andromalius.update(time_delta);

        for mut p in &mut particles {
            p.update(time_delta);
        }

        // Remove expired particles:
        particles.retain(|ref p| p.lifeclock.get_elapsed_time().as_seconds() < p.lifetime);

        // TODO: call Game.draw(&window) or something similar
        window.clear(&Color::new_rgb(200, 200, 200));
        window.draw(&player);
        window.draw(&andromalius);

        // p: &Particle
        for p in &particles {
            window.draw(p);
        }

        window.display();
    }
}
