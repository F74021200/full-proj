/*********************************************
Equation: N = (1/|G|) * sum (Xg)
Method: Add all Gx's after finding all taking.
*********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct CUBE_POINT cube_point;
typedef struct RECORD_POINT record_point;
typedef struct FACE face;
typedef struct CHECK_CUBE check_cube;
typedef struct METHOD_HEAD method_head;
typedef struct METHOD_POINT method_point;
typedef struct USING_POINT using_point;
typedef struct HEAD_METHOD_HEAD head_method_head;

struct CUBE_POINT {
	int cube_number;
	int face_number;
	int record_x;
	int record_y;
	int invalid;
	int set;
};
struct RECORD_POINT {
	int x;
	int y;
	int z;
};
struct FACE {
	record_point ****rotating_shaft;
	//1:x_rotate_shaft;
	//2:y_rotate_shaft;
	//3:z_rotate_shaft;
	//4:lbt_rfd_rotate_shaft;
	//5:rbt_lfd_rotate_shaft;
	//6:lft_rbd_rotate_shaft;
	//7:rft_lbd_rotate_shaft;
	//8:lt_rd_edge_rotate_shaft;
	//9:rt_ld_edge_rotate_shaft;
	//10:bt_fd_edge_rotate_shaft;
	//11:ft_bd_edge_rotate_shaft;
	//12:lf_rb_edge_rotate_shaft;
	//13:rf_lb_edge_rotate_shaft;
};
struct CHECK_CUBE {
	face *faces;
};
struct METHOD_HEAD {
	method_point *method;
	struct METHOD_HEAD *next;
};
struct METHOD_POINT {
	int x;
	int y;
	int z;
	struct METHOD_POINT *next;
};
struct USING_POINT {
	int x;
	int y;
	int z;
	struct USING_POINT *last;
	struct USING_POINT *next;
};
struct HEAD_METHOD_HEAD {
	struct METHOD_HEAD *method_head;
	struct HEAD_METHOD_HEAD *next;
};

void find_method (int , using_point *);
void build_rotate_table ();
void init ();
void determine_symmetry ();
void classify ();

int input_n;
check_cube *check_cubes = NULL;
cube_point ***cube = NULL;
using_point *using_points = NULL;
using_point *current_using_point_ptr;
using_point *index_using_point_ptr = NULL;
using_point *check_using_points = NULL, *equal_tmp_using_points = NULL;
using_point *index_check_using_point_ptr = NULL, *index_equal_tmp_using_point_ptr = NULL;
int index_invalid_x_int, index_invalid_y_int, index_invalid_z_int;
cube_point tmp_cube_point;
record_point tmp_record_point;
int record_equal_n = 0;
int index_shaft_x_int, index_shaft_y_int;
int tmp_x, tmp_y, tmp_z;
int *class_n_table;
int index_class_n_table;
int margin_n;
int margin_zero, margin_one, margin_two, margin_three;

method_head *all_method_top = NULL;
method_head *index_all_method_top_ptr = NULL, *index_current_all_method_top_ptr = NULL;
method_point *index_all_method_top_point_ptr = NULL, *index_equal_tmp_all_method_point_ptr = NULL;
int index_all_method_top_point_int;

int *Xg_table;
int rotate_n = 0;
int all_n = 0;

int main (int argc, char *argv[]) {

	if (argc != 2) {
		printf ("Incorrect insert\n");
		return 0;
	}

	using_point *index_using_points_ptr = NULL, initial_point;
	char *index_input_str_ptr;
	int index_Xg_table_int;
	clock_t t_start, t_end;
	int wanted;

	index_input_str_ptr = argv[1];
	input_n = 0;
	while (*index_input_str_ptr) {
		input_n = input_n * 10 + *index_input_str_ptr - 48;
		++index_input_str_ptr;
	}
	printf ("n: %d\n", input_n);

	init ();

	build_rotate_table ();

//printf ("initial\n");	
	index_using_points_ptr = using_points;

	t_start = clock ();

	initial_point.x = 0;
	initial_point.y = 0;
	initial_point.z = 0;
	current_using_point_ptr = using_points;
//printf ("find\n");
	find_method (input_n, &initial_point);

	index_all_method_top_ptr = all_method_top;
	for (index_all_method_top_point_int = 0; index_all_method_top_point_int < all_n; ++index_all_method_top_point_int) {
		determine_symmetry ();
		index_all_method_top_ptr = index_all_method_top_ptr->next;
	}

	for (index_Xg_table_int = 0; index_Xg_table_int < 48; ++index_Xg_table_int) {
		wanted += Xg_table[index_Xg_table_int];
	}
	//*****print the numbers of 7 kinds respectively -start
	printf ("all_n: %d\n", all_n);
	printf ("wanted: %d\n", wanted / 48);
	printf ("\n");
	t_end = clock ();
	printf ("Time: %lf\n", (t_end - t_start) / (double)(CLOCKS_PER_SEC));

	//*****print the numbers of 7 kinds respectively -end

	return 0;
}
void find_method (int remaining_point_n, using_point *last_point)
{
	int index_cube_x_int, index_cube_y_int, index_cube_z_int;
	int initial_x, initial_y, initial_z;
	initial_x = last_point->x;
	initial_y = last_point->y;
	initial_z = last_point->z;
	if (!remaining_point_n) {
		++all_n;

		//store the method -start
		if (!all_method_top) {
			all_method_top = malloc (sizeof (method_head));
			index_current_all_method_top_ptr = all_method_top;
		}
		else {
			index_current_all_method_top_ptr->next = malloc (sizeof (method_head));
			index_current_all_method_top_ptr = index_current_all_method_top_ptr->next;
		}
		index_current_all_method_top_ptr->next = NULL;
		index_using_point_ptr = using_points;
		for (index_all_method_top_point_int = 0; index_all_method_top_point_int < input_n; ++index_all_method_top_point_int) {
			if (!index_all_method_top_point_int) {
				index_current_all_method_top_ptr->method = malloc (sizeof (method_point));
				index_all_method_top_point_ptr = index_current_all_method_top_ptr->method;
			}
			else {
				index_all_method_top_point_ptr->next = malloc (sizeof (method_point));
				index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
			}
			index_all_method_top_point_ptr->x = index_using_point_ptr->x;
			index_all_method_top_point_ptr->y = index_using_point_ptr->y;
			index_all_method_top_point_ptr->z = index_using_point_ptr->z;
			index_using_point_ptr = index_using_point_ptr->next;
		}
		index_all_method_top_point_ptr->next = NULL;

		//store the method -end

	}
	else {
		for (index_cube_z_int = last_point->z; index_cube_z_int < input_n; ++index_cube_z_int) {
			for (index_cube_y_int = initial_y; index_cube_y_int < input_n; ++index_cube_y_int) {
				for (index_cube_x_int = initial_x; index_cube_x_int < input_n; ++index_cube_x_int) {


					if (!cube[index_cube_x_int][index_cube_y_int][index_cube_z_int].invalid) {
						//*****tag invalid points. -start
						for (index_invalid_y_int = 0; index_invalid_y_int < input_n; ++index_invalid_y_int) {
							for (index_invalid_z_int = 0; index_invalid_z_int < input_n; ++index_invalid_z_int) {
								++cube[index_cube_x_int][index_invalid_y_int][index_invalid_z_int].invalid;
							}
						}
						for (index_invalid_x_int = 0; index_invalid_x_int < input_n; ++index_invalid_x_int) {
							for (index_invalid_z_int = 0; index_invalid_z_int < input_n; ++index_invalid_z_int) {
								++cube[index_invalid_x_int][index_cube_y_int][index_invalid_z_int].invalid;
							}
						}
						for (index_invalid_x_int = 0; index_invalid_x_int < input_n; ++index_invalid_x_int) {
							for (index_invalid_y_int = 0; index_invalid_y_int < input_n; ++index_invalid_y_int) {
								++cube[index_invalid_x_int][index_invalid_y_int][index_cube_z_int].invalid;
							}
						}
						//*****tag invalid points. -end
						if (current_using_point_ptr->x != -1) {
							current_using_point_ptr = current_using_point_ptr->next;
						}
						current_using_point_ptr->x = index_cube_x_int;
						current_using_point_ptr->y = index_cube_y_int;
						current_using_point_ptr->z = index_cube_z_int;
						//using_cube_point_ptr = &cube[index_cube_x_int][index_cube_y_int][index_cube_z_int];
						find_method (remaining_point_n - 1, current_using_point_ptr);

						current_using_point_ptr->x = -1;
						if (current_using_point_ptr->last) {
							current_using_point_ptr = current_using_point_ptr->last;
						}
						//*****tag invalid points. -start
						for (index_invalid_y_int = 0; index_invalid_y_int < input_n; ++index_invalid_y_int) {
							for (index_invalid_z_int = 0; index_invalid_z_int < input_n; ++index_invalid_z_int) {
								--cube[index_cube_x_int][index_invalid_y_int][index_invalid_z_int].invalid;
							}
						}
						for (index_invalid_x_int = 0; index_invalid_x_int < input_n; ++index_invalid_x_int) {
							for (index_invalid_z_int = 0; index_invalid_z_int < input_n; ++index_invalid_z_int) {
								--cube[index_invalid_x_int][index_cube_y_int][index_invalid_z_int].invalid;
							}
						}
						for (index_invalid_x_int = 0; index_invalid_x_int < input_n; ++index_invalid_x_int) {
							for (index_invalid_y_int = 0; index_invalid_y_int < input_n; ++index_invalid_y_int) {
								--cube[index_invalid_x_int][index_invalid_y_int][index_cube_z_int].invalid;
							}
						}
						//*****tag invalid points. -end
					}
				}
				initial_x = 0;
			}
			initial_y = 0;
		}
	}
}
void build_rotate_table ()
{
	int index_cube_x_int, index_cube_y_int, index_cube_z_int;
	int index_edge_len;
	int index_record_face_x_int, index_record_face_y_int;
	int index_pair_int;

	//*****store each face -start
	//printf ("Start store each face\n");
	for (index_edge_len = input_n; index_edge_len > 0; index_edge_len = index_edge_len - 2) {
		index_cube_x_int = (input_n - index_edge_len)>>1;
		index_cube_y_int = index_cube_x_int;
		index_cube_z_int = index_cube_x_int;

		//***** Record the first face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int + index_record_face_x_int][index_cube_z_int].cube_number = index_cube_x_int;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int + index_record_face_x_int][index_cube_z_int].face_number = 0;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int + index_record_face_x_int][index_cube_z_int].record_x = index_record_face_x_int;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int + index_record_face_x_int][index_cube_z_int].record_y = index_record_face_y_int;
			}
		}
		//***** Record the first face in the cube. -end

		//***** Store the first face, which is the bottom face(add x then add y). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end

		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1- index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1- index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[0].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[0].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
		//***** Store the first face, which is the bottom face(add x then add y). -end

		//***** Record the second face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int][index_cube_y_int + index_record_face_y_int][index_cube_z_int + index_record_face_x_int].cube_number = index_cube_x_int;
				cube[index_cube_x_int][index_cube_y_int + index_record_face_y_int][index_cube_z_int + index_record_face_x_int].face_number = 1;
				cube[index_cube_x_int][index_cube_y_int + index_record_face_y_int][index_cube_z_int + index_record_face_x_int].record_x = index_record_face_x_int;
				cube[index_cube_x_int][index_cube_y_int + index_record_face_y_int][index_cube_z_int + index_record_face_x_int].record_y = index_record_face_y_int;
			}
		}
		//***** Record the second face in the cube. -end

		//***** Store the second face, which is the left face(add y then add z). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end
		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1- index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[1].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[1].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
		//***** Store the second face, which is the left face(add y then add z). -end

		//***** Record the third face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int][index_cube_z_int + index_record_face_x_int].cube_number = index_cube_x_int;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int][index_cube_z_int + index_record_face_x_int].face_number = 2;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int][index_cube_z_int + index_record_face_x_int].record_x = index_record_face_x_int;
				cube[index_cube_x_int + index_record_face_y_int][index_cube_y_int][index_cube_z_int + index_record_face_x_int].record_y = index_record_face_y_int;
			}
		}
		//***** Record the third face in the cube. -end

		//***** Store the third face, which is the font face(add x then add z). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end
		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[2].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[2].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
		//***** Store the third face, which is the font face(add x then add z). -end

		//***** Record the forth face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int + index_edge_len - 1][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].cube_number = index_cube_x_int;
				cube[index_cube_x_int + index_edge_len - 1][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].face_number = 3;
				cube[index_cube_x_int + index_edge_len - 1][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].record_x = index_record_face_x_int;
				cube[index_cube_x_int + index_edge_len - 1][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].record_y = index_record_face_y_int;
			}
		}
		//***** Record the forth face in the cube. -end

		//***** Store the forth face, which is the right face(begin at (n, n, n), substract z then substract y). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end

		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[3].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[3].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
		//***** Store the forth face, which is the right face(begin at (n, n, n), substract z then substract y). -end

		//***** Record the fifth face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_x_int][index_cube_y_int + index_edge_len - 1][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].cube_number = index_cube_x_int;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_x_int][index_cube_y_int + index_edge_len - 1][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].face_number = 4;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_x_int][index_cube_y_int + index_edge_len - 1][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].record_x = index_record_face_x_int;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_x_int][index_cube_y_int + index_edge_len - 1][index_cube_z_int + index_edge_len - 1 - index_record_face_y_int].record_y = index_record_face_y_int;
			}
		}
		//***** Record the fifth face in the cube. -end

		//***** Store the fifth face, which is the back face(begin at (n, n, n), substract z then substract x). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end

		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[4].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[4].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
		//***** Store the fifth face, which is the back face(begin at (n, n, n), substract z then substract x). -end

		//***** Record the sixth face in the cube. -start
		for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_y_int][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1].cube_number = index_cube_x_int;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_y_int][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1].face_number = 5;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_y_int][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1].record_x = index_record_face_x_int;
				cube[index_cube_x_int + index_edge_len - 1 - index_record_face_y_int][index_cube_y_int + index_edge_len - 1 - index_record_face_x_int][index_cube_z_int + index_edge_len - 1].record_y = index_record_face_y_int;
			}
		}
		//***** Record the sixth face in the cube. -end

		//***** Store the sixth face, which is the top face(begin at (n, n, n), substract x then substract y). -start
		//*****X rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****X rotating shaft -end

		//*****Y rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[1][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Y rotating shaft -end

		//*****Z rotating shaft -start
		//printf ("Start X rotating shaft\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[2][2][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][2][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//printf ("End X rotating shaft\n");
		//*****Z rotating shaft -end

		//*****lbt_rfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[3][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lbt_rfd rotating shaft -end

		//*****rbt_lfd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[4][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rbt_lfd rotating shaft -end

		//*****lft_rbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[5][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lft_rbd rotating shaft -end

		//*****rft_lbd rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[6][1][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][1][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rft_lbd rotating shaft -end

		//*****lt_rd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[7][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lt_rd_edge_rotating shaft -end

		//*****rt_ld_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_y_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[8][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rt_ld_edge_rotating shaft -end

		//*****bt_fd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_edge_len - 1 - index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[9][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****bt_fd_edge_rotating shaft -end

		//*****ft_bd_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int + index_record_face_x_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[10][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****ft_bd_edge_rotating shaft -end

		//*****lf_rb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_edge_len - 1 - index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_edge_len - 1 - index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[11][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****lf_rb_edge_rotating shaft -end

//this
		//*****rf_lb_edge_rotating shaft -start
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][index_pair_int] = malloc (sizeof (record_point*) * index_edge_len);
		}
//printf ("test\n");
		for (index_pair_int = 0; index_pair_int < 3; ++index_pair_int) {
			for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][index_pair_int][index_record_face_x_int] = malloc (sizeof (record_point) * index_edge_len);
			}
		}
		for (index_record_face_x_int = 0; index_record_face_x_int < index_edge_len; ++index_record_face_x_int) {
			for (index_record_face_y_int = 0; index_record_face_y_int < index_edge_len; ++index_record_face_y_int) {
			if (index_edge_len != 1) {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int + index_record_face_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int + index_record_face_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			else {
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].x = index_cube_x_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].y = index_cube_y_int;
				check_cubes[index_cube_x_int].faces[5].rotating_shaft[12][0][index_record_face_x_int][index_record_face_y_int].z = index_cube_z_int;
			}
			/*
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].x);
			printf ("%d ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].y);
			printf ("%d; ", check_cubes[index_cube_x_int].faces[5].rotating_shaft[0][0][index_record_face_x_int][index_record_face_y_int].z);
			*/
			}
		}
		//printf ("\n\n");
		//*****rf_lb_edge_rotating shaft -end
	}
	//printf ("End store each face\n");
	//*****store each face -end
}
void init ()
{
	int check_cubes_n;
	using_point *index_using_points_ptr = NULL;
	int index_cube_x_int, index_cube_y_int, index_cube_z_int;
	int index_check_cubes;
	int index_using_points_int;
	int index_face_int;
	int index_Xg_table_int;

	class_n_table = malloc (sizeof (int) * 7);
	for (index_class_n_table = 0; index_class_n_table < 7; ++index_class_n_table) {
		class_n_table[index_class_n_table] = 0;
	}


	//*****malloc and initialize check_cubes -start
	//printf ("Start to malloc and initialize check_cubes\n");
	check_cubes_n = (input_n + 1)>>1;
	check_cubes = malloc (sizeof (check_cube) * check_cubes_n);
	for (index_check_cubes = 0; index_check_cubes < check_cubes_n; ++index_check_cubes) {
		check_cubes[index_check_cubes].faces = malloc (sizeof (face) * 6);;
	}
	for (index_check_cubes = 0; index_check_cubes < check_cubes_n; ++index_check_cubes) {
		for (index_face_int = 0; index_face_int < 6; ++index_face_int){
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft = malloc (sizeof (record_point***) * 13);
		}
	}
	for (index_check_cubes = 0; index_check_cubes < check_cubes_n; ++index_check_cubes) {
		for (index_face_int = 0; index_face_int < 6; ++index_face_int){
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[0] = malloc (sizeof (record_point**) * 3); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[1] = malloc (sizeof (record_point**) * 3); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[2] = malloc (sizeof (record_point**) * 3); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[3] = malloc (sizeof (record_point**) * 2); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[4] = malloc (sizeof (record_point**) * 2); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[5] = malloc (sizeof (record_point**) * 2); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[6] = malloc (sizeof (record_point**) * 2); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[7] = malloc (sizeof (record_point**)); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[8] = malloc (sizeof (record_point**)); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[9] = malloc (sizeof (record_point**)); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[10] = malloc (sizeof (record_point**)); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[11] = malloc (sizeof (record_point**)); 
			check_cubes[index_check_cubes].faces[index_face_int].rotating_shaft[12] = malloc (sizeof (record_point**)); 
		}
	}
	//printf ("End to malloc and initialize check_cubes\n");
	//*****malloc and initialize check_cubes -end


	//*****malloc cube -start
	//printf ("Start to malloc cube\n");
	cube = malloc (sizeof (cube_point**) * input_n);
	for (index_cube_x_int = 0; index_cube_x_int < input_n; ++index_cube_x_int) {
		cube[index_cube_x_int] = malloc (sizeof (cube_point*) * input_n);
	}
	for (index_cube_x_int = 0; index_cube_x_int < input_n; ++index_cube_x_int) {
		for (index_cube_y_int = 0; index_cube_y_int < input_n; ++index_cube_y_int) {
			cube[index_cube_x_int][index_cube_y_int] = malloc (sizeof (cube_point) * input_n);
		}
	}
	for (index_cube_x_int = 0; index_cube_x_int < input_n; ++index_cube_x_int){
		for (index_cube_y_int = 0; index_cube_y_int < input_n; ++index_cube_y_int){
			for (index_cube_z_int = 0; index_cube_z_int < input_n; ++index_cube_z_int){
				cube[index_cube_x_int][index_cube_y_int][index_cube_z_int].face_number = -1;
				cube[index_cube_x_int][index_cube_y_int][index_cube_z_int].invalid = 0;
				//cube[index_cube_x_int][index_cube_y_int][index_cube_z_int].face_number = -1;
			}
		}
	}
	//printf ("End to malloc cube\n");
	//*****malloc and cube -end

	//*****malloc using_points -start
	using_points = NULL;

	check_using_points = NULL;
	equal_tmp_using_points = NULL;

	for (index_using_points_int = 0; index_using_points_int < input_n; ++index_using_points_int) {
		if (!using_points) {
			using_points = malloc (sizeof (using_point));
			index_using_points_ptr = using_points;
			index_using_points_ptr->last = NULL;
			check_using_points = malloc (sizeof (using_point));
			index_check_using_point_ptr = check_using_points;
			index_check_using_point_ptr->last = NULL;
			equal_tmp_using_points = malloc (sizeof (using_point));
			index_equal_tmp_using_point_ptr = equal_tmp_using_points;
			index_equal_tmp_using_point_ptr->last = NULL;
		}
		else {
			index_using_points_ptr->next = malloc (sizeof (using_point));
			index_using_points_ptr->next->last = index_using_points_ptr;
			index_using_points_ptr = index_using_points_ptr->next;
			index_check_using_point_ptr->next = malloc (sizeof (using_point));
			index_check_using_point_ptr->next->last = index_check_using_point_ptr;
			index_check_using_point_ptr = index_check_using_point_ptr->next;
			index_equal_tmp_using_point_ptr->next = malloc (sizeof (using_point));
			index_equal_tmp_using_point_ptr->next->last = index_equal_tmp_using_point_ptr;
			index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
		}

		index_using_points_ptr->next = NULL;
		index_using_points_ptr->x = -1;
		index_using_points_ptr->y = -1;
		index_using_points_ptr->z = -1;
//printf ("malloc start\n");
		index_check_using_point_ptr->next = NULL;
		index_check_using_point_ptr->x = -1;
		index_check_using_point_ptr->y = -1;
		index_check_using_point_ptr->z = -1;
		index_equal_tmp_using_point_ptr->next = NULL;
		index_equal_tmp_using_point_ptr->x = -1;
		index_equal_tmp_using_point_ptr->y = -1;
		index_equal_tmp_using_point_ptr->z = -1;
	}
//printf ("malloc end\n");
	//*****malloc using_points -end

	//*****malloc Xg_table -start

	Xg_table = malloc (sizeof (int) * 48);
	for (index_Xg_table_int = 0; index_Xg_table_int < 48; ++index_Xg_table_int) {
		Xg_table[index_Xg_table_int] = 0;
	}

	//*****malloc Xg_clss_table -end
}
void determine_symmetry ()
{

	//find -start

//printf ("test 1\n");
		rotate_n = 0;

		++Xg_table[rotate_n];

		index_equal_tmp_using_point_ptr = equal_tmp_using_points;
		index_all_method_top_point_ptr = index_all_method_top_ptr->method;
		while (index_equal_tmp_using_point_ptr) {
			index_equal_tmp_using_point_ptr->x = input_n - 1 - index_all_method_top_point_ptr->x;
			index_equal_tmp_using_point_ptr->y = index_all_method_top_point_ptr->y;
			index_equal_tmp_using_point_ptr->z = index_all_method_top_point_ptr->z;
			index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
			index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
		}
		++rotate_n;
//printf ("test 2\n");
		index_equal_tmp_using_point_ptr = equal_tmp_using_points;
		record_equal_n = 0;
		while (index_equal_tmp_using_point_ptr) {
			index_all_method_top_point_ptr = index_all_method_top_ptr->method;
			while (index_all_method_top_point_ptr) {
				if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
					++record_equal_n;
				}
				index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
			}
			index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
		}
		if (record_equal_n == input_n) {
			++Xg_table[rotate_n];
		}
//printf ("test 3\n");
		for (index_shaft_x_int = 0; index_shaft_x_int < 3; ++index_shaft_x_int){
			for (index_shaft_y_int = 0; index_shaft_y_int < 3; ++index_shaft_y_int){
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				index_all_method_top_point_ptr = index_all_method_top_ptr->method;
				while (index_all_method_top_point_ptr) {
					tmp_cube_point = cube[index_all_method_top_point_ptr->x][index_all_method_top_point_ptr->y][index_all_method_top_point_ptr->z];
					tmp_record_point = check_cubes[tmp_cube_point.cube_number].faces[tmp_cube_point.face_number].rotating_shaft[index_shaft_x_int][index_shaft_y_int][tmp_cube_point.record_x][tmp_cube_point.record_y];
					index_equal_tmp_using_point_ptr->x = tmp_record_point.x;
					index_equal_tmp_using_point_ptr->y = tmp_record_point.y;
					index_equal_tmp_using_point_ptr->z = tmp_record_point.z;
					index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				++rotate_n;
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
				++rotate_n;
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				while (index_equal_tmp_using_point_ptr) {
					index_equal_tmp_using_point_ptr->x = input_n - 1 - index_equal_tmp_using_point_ptr->x;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
			}
		}
//printf ("test 4\n");
		for (index_shaft_x_int = 3; index_shaft_x_int < 7; ++index_shaft_x_int){
			for (index_shaft_y_int = 0; index_shaft_y_int < 2; ++index_shaft_y_int){
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				index_all_method_top_point_ptr = index_all_method_top_ptr->method;
				++rotate_n;
				while (index_all_method_top_point_ptr) {
					tmp_cube_point = cube[index_all_method_top_point_ptr->x][index_all_method_top_point_ptr->y][index_all_method_top_point_ptr->z];
					tmp_record_point = check_cubes[tmp_cube_point.cube_number].faces[tmp_cube_point.face_number].rotating_shaft[index_shaft_x_int][index_shaft_y_int][tmp_cube_point.record_x][tmp_cube_point.record_y];
					index_equal_tmp_using_point_ptr->x = tmp_record_point.x;
					index_equal_tmp_using_point_ptr->y = tmp_record_point.y;
					index_equal_tmp_using_point_ptr->z = tmp_record_point.z;
					index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				++rotate_n;
				while (index_equal_tmp_using_point_ptr) {
					index_equal_tmp_using_point_ptr->x = input_n - 1 - index_equal_tmp_using_point_ptr->x;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
			}
		}
		for (index_shaft_x_int = 7; index_shaft_x_int < 13; ++index_shaft_x_int){
			for (index_shaft_y_int = 0; index_shaft_y_int < 1; ++index_shaft_y_int){
				++rotate_n;
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				index_all_method_top_point_ptr = index_all_method_top_ptr->method;
				while (index_all_method_top_point_ptr) {
					tmp_cube_point = cube[index_all_method_top_point_ptr->x][index_all_method_top_point_ptr->y][index_all_method_top_point_ptr->z];
					tmp_record_point = check_cubes[tmp_cube_point.cube_number].faces[tmp_cube_point.face_number].rotating_shaft[index_shaft_x_int][index_shaft_y_int][tmp_cube_point.record_x][tmp_cube_point.record_y];
					index_equal_tmp_using_point_ptr->x = tmp_record_point.x;
					index_equal_tmp_using_point_ptr->y = tmp_record_point.y;
					index_equal_tmp_using_point_ptr->z = tmp_record_point.z;
					index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				++rotate_n;
				while (index_equal_tmp_using_point_ptr) {
					index_equal_tmp_using_point_ptr->x = input_n - 1 - index_equal_tmp_using_point_ptr->x;
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				index_equal_tmp_using_point_ptr = equal_tmp_using_points;
				record_equal_n = 0;
				while (index_equal_tmp_using_point_ptr) {
					index_all_method_top_point_ptr = index_all_method_top_ptr->method;
					while (index_all_method_top_point_ptr) {
						if (index_all_method_top_point_ptr->x == index_equal_tmp_using_point_ptr->x && index_all_method_top_point_ptr->y == index_equal_tmp_using_point_ptr->y && index_all_method_top_point_ptr->z == index_equal_tmp_using_point_ptr->z) {
							++record_equal_n;
						}
						index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
					}
					index_equal_tmp_using_point_ptr = index_equal_tmp_using_point_ptr->next;
				}
				if (record_equal_n == input_n) {
					++Xg_table[rotate_n];
				}
			}
		}
//		printf ("rotate_n: %d\n", rotate_n);
	//find -end
}
void classify ()
{

		margin_one = 0;
		margin_two = 0;
		margin_three = 0;
		while (index_all_method_top_point_ptr) {
			margin_n = 0;
			if (index_all_method_top_point_ptr->x == 0 || index_all_method_top_point_ptr->x == input_n - 1) ++margin_n;
			if (index_all_method_top_point_ptr->y == 0 || index_all_method_top_point_ptr->y == input_n - 1) ++margin_n;
			if (index_all_method_top_point_ptr->z == 0 || index_all_method_top_point_ptr->z == input_n - 1) ++margin_n;

			if (margin_n == 1) {
				++margin_one;
			}
			else if (margin_n == 2) {
				++margin_two;
			}
			else if (margin_n == 3) {
				++margin_three;
			}

			index_all_method_top_point_ptr = index_all_method_top_point_ptr->next;
		}

		//printf ("margin: %d, %d, %d\n", margin_one, margin_two, margin_three);
		if (margin_one == 0 && margin_two == 0 && margin_three == 2) {
			index_class_n_table = 0;
		}
		else if (margin_one == 2 && margin_two == 2 && margin_three == 0) {
			index_class_n_table = 1;
		}
		else if (margin_one == 0 && margin_two == 3 && margin_three == 0) {
			index_class_n_table = 2;
		}
		else if (margin_one == 1 && margin_two == 1 && margin_three == 1) {
			index_class_n_table = 3;
		}
		else if (margin_one == 3 && margin_two == 0 && margin_three == 1) {
			index_class_n_table = 4;
		}
		else if (margin_one == 4 && margin_two == 1 && margin_three == 0) {
			index_class_n_table = 5;
		}
		else if (margin_one == 6 && margin_two == 0 && margin_three == 0) {
			index_class_n_table = 6;
		}
}
