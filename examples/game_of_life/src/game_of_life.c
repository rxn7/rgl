#include "rgl/rgl.h"
#include "rgl/rgl_v2.h"

#define CELL_ROWS 100
#define CELL_COUNT CELL_ROWS * CELL_ROWS
#define CELL_GAP 0.2 
#define BOARD_SIZE 1000
#define HALF_BOARD_SIZE BOARD_SIZE * 0.5f
#define UPDATE_INTERVAL 0.1f

static bool cells[CELL_COUNT];
static bool cells_temp[CELL_COUNT];
static rglVao cells_vao;
static float update_timer = 0.0f;

void appInit(void);
void appQuit(void);
void appUpdate(f32 dt);
void appDraw(void);
void updateCells(void); u8 countNeighbours(u32 idx); void randomizeCells();

int 
main(int argc, const char **argv) {
	memset(cells, 0, CELL_COUNT * sizeof(bool));

	rglAppDesc desc = (rglAppDesc) {
		.width = BOARD_SIZE,
		.height = BOARD_SIZE, .title = "RGL | Game of Life",
		.background_color = RGL_BLACK,
		.update_f = appUpdate,
		.draw_f = appDraw,
		.init_f = appInit, .quit_f = appQuit,
	};

	rglStart(&desc);
}

void
appInit(void) {
	rglVaoCreate(&cells_vao, nullptr, 0);
	randomizeCells();
}

void
appQuit(void) {
}

void
appUpdate(f32 dt) {
	_rgl_camera->zoom += _rgl_scroll_value * 0.05f;
	rglMathClamp(_rgl_camera->zoom, .1f, 3.f);

	update_timer += dt;

	if(update_timer >= UPDATE_INTERVAL) {
		update_timer = 0.0f;
		updateCells();
	} 
}

void 
appDraw(void) {
	const float camera_size = _rgl_camera->right - _rgl_camera->left;
	const float half_camera_size = camera_size * 0.5f;
	const float cell_size = camera_size / CELL_ROWS;
	const float half_cell_size = cell_size * 0.5f;

	for(u32 i = 0; i<CELL_COUNT; ++i) {
		if(!cells[i])
			continue;

		f32 x = (f32)i / CELL_ROWS * cell_size - half_camera_size;
		f32 y = (i % CELL_ROWS) * cell_size - half_camera_size;

		rglDrawRect(RGL_WHITE, (rglV2){.x = x - half_cell_size, .y = y - half_cell_size}, (rglV2){.x = half_cell_size - CELL_GAP * cell_size, .y = half_cell_size - CELL_GAP * cell_size});
	}
}

u8 
countNeighbours(u32 idx) {
	u8 count = 0;

	i32 cell_x = idx % CELL_ROWS;
	i32 cell_y = idx / CELL_ROWS;

	for(i32 of_y = -1 ; of_y <= 1; ++of_y) {
		for(i32 of_x = -1 ; of_x <= 1; ++of_x) {
			i32 x = cell_x + of_x;
			i32 y = cell_y + of_y;

			if(of_x == 0 && of_y == 0)
				continue;

			// out of bounds
			if(x == -1 || x == CELL_ROWS || y == -1 || y == CELL_ROWS)
				continue;

			if(cells[y * CELL_ROWS + x])
				count++;
		}
	}

	return count;
}

void 
randomizeCells(void) {
	for(u32 i = 0; i<CELL_COUNT; ++i) {
		cells[i] = rand() % 2;
	}
}

void
updateCells(void) {
	for(u32 i=0; i<CELL_COUNT; ++i)
		cells_temp[i] = cells[i];

	for(u32 i = 0; i<CELL_COUNT; ++i) {
		const u8 neighbourCount = countNeighbours(i);

		if(cells[i]) {
			if(neighbourCount < 2 || neighbourCount > 3)
				cells_temp[i] = false;
		} else if(neighbourCount == 3) {
			cells_temp[i] = true;
		}
	}

	for(u32 i=0; i<CELL_COUNT; ++i)
		cells[i] = cells_temp[i];
}
