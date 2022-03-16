# TODO

## Sprite Animation System
### rgl_sprite_animation_t 
- texture *texture
- u16 *frames
- u16 frame_count
- v2 frame_size (In pixels)
- f32 seconds_per_frame
- b8 looping

### rgl_sprite_animator_t
- sprite *target
- animation *current_animation, *next_animation
- u16 current_frame
- f32 time_elapsed
