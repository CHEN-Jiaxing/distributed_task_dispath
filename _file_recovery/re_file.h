#include <vector>

using std::vector;

int csv_recovery(int task_id);
int create_block(int task_id, int node_id);
vector<int> sort_blocks(int task_id, int node_id);
void sort_nodes(int task_id, int node_id, int block_num);
void sort_file(int task_id, int max_line);