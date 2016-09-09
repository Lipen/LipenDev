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
pub mod stats;
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

    let mut players: Vec<Box<Player>> = vec![];

    // Create player and setup its animations
    {
        let mut player = Player::new(Vector2f::new(300., 400.), 200., Vector2f::new(64., 64.));
        player.set_texture(texture_manager.get(TextureIdentifier::Wizard), false);
        player.set_origin(&Vector2f::new(32., 61.));
        player.add_animation_frame(AnimationIdentifier::Stay, 0, 16, 0.08);
        player.add_animation_frame(AnimationIdentifier::MoveLeft, 1, 16, 0.08);
        player.add_animation_frame(AnimationIdentifier::MoveRight, 2, 16, 0.08);
        player.add_animation_frame(AnimationIdentifier::MoveUp, 3, 16, 0.08);
        player.add_animation_frame(AnimationIdentifier::MoveDown, 0, 16, 0.08);
        players.push(Box::new(player));
    }

    // Create enemy
    {
        let mut andromalius = Player::new(Vector2f::new(500., 100.), 100., Vector2f::new(57., 88.));
        andromalius.set_texture(texture_manager.get(TextureIdentifier::Andromalius), false);
        andromalius.set_origin(&Vector2f::new(57. / 2., 80.));
        andromalius.add_animation_frame(AnimationIdentifier::Stay, 3, 8, 0.2);
        andromalius.add_animation_frame(AnimationIdentifier::MoveLeft, 4, 8, 0.15);
        andromalius.add_animation_frame(AnimationIdentifier::MoveRight, 0, 8, 0.15);
        players.push(Box::new(andromalius));
    }

    // Initialize particles vector
    let mut particles: Vec<Box<Particle>> = vec![];

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
                            println!("[d] player.get_position():\n\t{:?}",
                                     (*players[0]).get_position());
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
                        {
                            let mut particle = Particle::new((*players[0]).get_position(),
                                                             300.,
                                                             Vector2f::new(128., 128.),
                                                             3.);
                            particle.set_texture(texture_manager.get(TextureIdentifier::Particle), false);
                            particle.set_scale2f(0.25, 0.25);
                            particle.add_animation_frame(AnimationIdentifier::Move, 0, 26, 0.1);
                            particle.order(Order::Move {
                                x: x as f32,
                                y: y as f32,
                            });

                            particles.push(Box::new(particle));
                            println!("Particle spawned!");
                        }

                        (*players[0]).stats.health *= 0.95;
                    }

                    if let MouseButton::Right = button {
                        if Key::LControl.is_pressed() {
                            (*players[1]).order(Order::Move {
                                x: x as f32,
                                y: y as f32,
                            })
                        } else {
                            (*players[0]).order(Order::Move {
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

        // Remove expired particles:
        particles.retain(|ref p| p.lifeclock.get_elapsed_time().as_seconds() < p.lifetime);

        // Update particles
        for p in particles.iter_mut() {
            p.update(time_delta);
        }
        // Update players
        for ent in players.iter_mut() {
            ent.update(time_delta);
        }


        // Collisions check
        let n = particles.len();
        for i in 1..n {
            let (left, right) = (&mut particles[..]).split_at_mut(i+1);
            let mut p1 = &mut left[i];
            for j in (i+1)..n {
                let mut p2 = &mut right[j-i-1];
                p1.collide_with(&mut p2);
            }
        }

        // for player in players.iter_mut() {
        {
            let player = &mut players[1];
            for particle in particles.iter_mut() {
                player.collide_with(particle);
            }
        }


        // Draw everything
        window.clear(&Color::new_rgb(200, 200, 200));
        for p in players.iter() {
            window.draw(&**p);
        }

        for p in particles.iter() {
            window.draw(&**p);
        }

        window.display();
    }
}
