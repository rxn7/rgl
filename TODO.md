# TODO

## UI System
## Text
## Buttons
## Input Fields

## Sprite Animation System
### rglSpriteAnimation
- texture *texture
- u16 *frames
- u16 frame_count
- v2 frame_size (In pixels)
- f32 seconds_per_frame
- b8 looping

### rglSpriteAnimator
- sprite *target
- animation *default_animation, *current_animation, *next_animation
- u16 current_frame
- f32 time_elapsed
