pub struct Stats {
    pub health: f32,
	pub maxhealth: f32,
}

impl Stats {
    pub fn new(health: f32, maxhealth: f32) -> Self {
        Stats{
            health: health,
            maxhealth: maxhealth,
        }
    }
}
