#pragma once

/*!
 * @file generate.h
 * @brief �_���W�������������̃w�b�_�[�t�@�C��
 * @date 2014/08/08
 * @author
 * �s��(�ϋ�ؓ{�X�^�b�t�H)
 */

#define SAFE_MAX_ATTEMPTS 5000 /*!< ����������{���s�� */



extern int dun_tun_rnd; /*!< �_���W�����̒ʘH������~���񂷕p�x(���̎��s���Ƃ�%�Ŕ��肵�Ă���) */
extern int dun_tun_chg; /*!< �_���W�����̒ʘH���N�����N������p�x(���̎��s���Ƃ�%�Ŕ��肵�Ă���) */
extern int dun_tun_con; /*!< �_���W�����̒ʘH���p�����Ĉ������΂��p�x(���̎��s���Ƃ�%�Ŕ��肵�Ă���) */
extern int dun_tun_pen; /*!< �_���W�����̕��������Ƀh�A��ݒu����p�x(���̎��s���Ƃ�%�Ŕ��肵�Ă���) */
extern int dun_tun_jct; /*!< �_���W�����̒ʘH�����n�_�t�߂Ƀh�A��ݒu����p�x(���̎��s���Ƃ�%�Ŕ��肵�Ă���) */

/*
 * Hack -- Dungeon allocation "places"
 */
#define ALLOC_SET_CORR		1	/* Hallway */
#define ALLOC_SET_ROOM		2	/* Room */
#define ALLOC_SET_BOTH		3	/* Anywhere */

 /*
  * Hack -- Dungeon allocation "types"
  */
#define ALLOC_TYP_RUBBLE	1	/* Rubble */
#define ALLOC_TYP_TRAP		3	/* Trap */
#define ALLOC_TYP_GOLD		4	/* Gold */
#define ALLOC_TYP_OBJECT	5	/* Object */
#define ALLOC_TYP_INVIS		6	/* Invisible wall */



  /*
   * The "size" of a "generation block" in grids
   */
#define BLOCK_HGT	11
#define BLOCK_WID	11

   /*
	* Maximum numbers of rooms along each axis (currently 6x6)
	*/
#define MAX_ROOMS_ROW	(MAX_HGT / BLOCK_HGT)
#define MAX_ROOMS_COL	(MAX_WID / BLOCK_WID)


	/*
	 * Bounds on some arrays used in the "dun_data" structure.
	 * These bounds are checked, though usually this is a formality.
	 */
#define CENT_MAX	100
#define DOOR_MAX	200
#define WALL_MAX	500
#define TUNN_MAX	900


	 /*
	  * Structure to hold all "dungeon generation" data
	  */

typedef struct dun_data dun_data;

struct dun_data
{
	/* Array of centers of rooms */
	int cent_n;
	coord cent[CENT_MAX];

	/* Array of possible door locations */
	int door_n;
	coord door[DOOR_MAX];

	/* Array of wall piercing locations */
	int wall_n;
	coord wall[WALL_MAX];

	/* Array of tunnel grids */
	int tunn_n;
	coord tunn[TUNN_MAX];

	/* Number of blocks along each axis */
	int row_rooms;
	int col_rooms;

	/* Array of which blocks are used */
	bool room_map[MAX_ROOMS_ROW][MAX_ROOMS_COL];

	/* Various type of dungeon floors */
	bool destroyed;
	bool empty_level;
	bool cavern;
	int laketype;
};

extern dun_data *dun;

extern bool place_quest_monsters(void);
extern void wipe_generate_random_floor_flags(void);
extern void clear_cave(void);
extern void generate_random_floor(void);

extern bool build_tunnel(POSITION row1, POSITION col1, POSITION row2, POSITION col2);
extern bool build_tunnel2(POSITION x1, POSITION y1, POSITION x2, POSITION y2, int type, int cutoff);