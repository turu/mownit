#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define INF 1000000001

int readInteger()
{
	int ans = 0;
	char c;
	bool got = false;
	while(1)
	{
		c = getchar_unlocked();
		if(c >= '0' && c <= '9'){
			ans = ans * 10 + c - '0';
			got = true;
		} else if(got) {
			break;
		}
	}
	return ans;
}

//structure for holding points in 3d space; t[0 ... 2] holds x, y, z coordinates
int CT = 0; //ordering dimension 0..2
struct POINT{
	unsigned char t[3];

	bool operator< (const POINT & rhs){
		return t[CT] < rhs.t[CT];
	}

	bool operator== (const POINT & rhs){
		return t[CT] == rhs.t[CT];
	}
};

bool pcomp (const POINT & lhs, const POINT & rhs){
	return lhs.t[CT] < rhs.t[CT];
}

struct KDNode{
	KDNode * parent; //pointer to the parent
	KDNode * left_child; //pointer to the left child
	KDNode * right_child; //pointer to the right child
	POINT key; //POINT object that this node represents
	POINT min; //minimal coordinate values in node's subtree
	POINT max; //maximal coordinate values in node's subtree
	int order_axis; //says by which axis was a set ordered while using this node as a splitting point
};

//builds KD-Tree (3D in this particular case) from a set of POINT objects and returns pointer to the root
//the procedure utilizes fast median finding algorithm O(n) to determine splitting points in each step
//predicted time complexity O(n log n)
KDNode * KDTree(POINT * list, int beg, int end, KDNode * par = NULL, int depth = 0){
	int i=0, med = (beg + end) / 2;
	KDNode * node = new KDNode();
	node->parent = par;
	node->order_axis = depth % 3; //mod k in general k-dimensional case
	CT = node->order_axis;
	std::nth_element(list + beg, list + med, list + end, pcomp);
	node->key = list[med];

	node->min = node->max = list[beg];
	for(i = beg + 1; i < end; i++){
		if(list[i].t[0] < node->min.t[0])node->min.t[0] = list[i].t[0];
		if(list[i].t[1] < node->min.t[1])node->min.t[1] = list[i].t[1];
		if(list[i].t[2] < node->min.t[2])node->min.t[2] = list[i].t[2];
		if(list[i].t[0] > node->max.t[0])node->max.t[0] = list[i].t[0];
		if(list[i].t[1] > node->max.t[1])node->max.t[1] = list[i].t[1];
		if(list[i].t[2] > node->max.t[2])node->max.t[2] = list[i].t[2];
	}

	//build both left and right subtrees of current node, by recursively executing KDTree procedure
	if(beg < med)
		node->left_child = KDTree(list, beg, med, node, depth + 1);
	else
		node->left_child = NULL;
	if(med + 1 < end)
		node->right_child = KDTree(list, med + 1, end, node, depth + 1);
	else
		node->right_child = NULL;
	return node;
}

//euclidean distance between two points
inline int distance(POINT * lhs, POINT * rhs){
	int xd = lhs->t[0] - rhs->t[0];
	int yd = lhs->t[1] - rhs->t[1];
	int zd = lhs->t[2] - rhs->t[2];
	return xd * xd + yd * yd + zd * zd;
}

inline KDNode * maxcolor(KDNode * lhs, KDNode * rhs){
	if(lhs->key.t[0] > rhs->key.t[0]){
		return lhs;
	}else if(lhs->key.t[0] == rhs->key.t[0]){
		if(lhs->key.t[1] > rhs->key.t[1]){
			return lhs;
		}else if(lhs->key.t[1] == rhs->key.t[1]){
			if(lhs->key.t[2] > rhs->key.t[2]){
				return lhs;
			}else{
				return rhs;
			}
		}else{
			return rhs;
		}
	}else{
		return rhs;
	}
}

inline int distance_axis(POINT * lhs, POINT * rhs, int axis){
	int d = lhs->t[axis] - rhs->t[axis];
	return d*d;
}

inline int lower_bound(KDNode * root, POINT * point){
	POINT reduced;
	if(point->t[0] >= root->min.t[0] && point->t[0] <= root->max.t[0])reduced.t[0] = point->t[0];
	else if(point->t[0] < root->min.t[0])
		reduced.t[0] = root->min.t[0];
	else
		reduced.t[0] = root->max.t[0];
	if(point->t[1] >= root->min.t[1] && point->t[1] <= root->max.t[1])reduced.t[1] = point->t[1];
	else if(point->t[1] < root->min.t[1])
		reduced.t[1] = root->min.t[1];
	else
		reduced.t[1] = root->max.t[1];
	if(point->t[2] >= root->min.t[2] && point->t[2] <= root->max.t[2])reduced.t[2] = point->t[2];
	else if(point->t[2] < root->min.t[2])
		reduced.t[2] = root->min.t[2];
	else
		reduced.t[2] = root->max.t[2];
	return distance(&reduced, point);

}

int bestdist = INF;
KDNode * KDSearchNN(KDNode * here, POINT * point, KDNode * best = NULL){
	if(here == NULL || bestdist == 0)return best;
	//compute lower bound of subtree and prone
	if(lower_bound(here, point) > bestdist)return best;
	CT = here->order_axis;
	bool plh = *point < here->key;
	bool hlp = here->key < *point;
	if(!hlp)best = KDSearchNN(here->left_child, point, best);
	if(!plh)best = KDSearchNN(here->right_child, point, best);
	int curdist = distance(&here->key, point);
	if(curdist < bestdist){
		best = here;
		bestdist = curdist;
	}else if(curdist == bestdist){
		best = maxcolor(here, best);
	}
	if(bestdist == 0)return best;
	if(distance_axis(&here->key, point, here->order_axis) <= bestdist){
		if(hlp)best = KDSearchNN(here->left_child, point, best);
		if(plh)best = KDSearchNN(here->right_child, point, best);
	}
	return best;
}

int t, n, m;
KDNode * root, * match;
POINT paleta[30000], obraz[50000];
POINT point;

int main(){
	int i;
        t = readInteger();
	while(t--){
		n = readInteger();
		for(i = 0; i < n; i++){
                        paleta[i].t[0] = readInteger();
                        paleta[i].t[1] = readInteger();
                        paleta[i].t[2] = readInteger();
		}
		root = KDTree(paleta, 0, n);
		m = readInteger();
		for(i = 0; i < m; i++){
			point.t[0] = readInteger();
                        point.t[1] = readInteger();
                        point.t[2] = readInteger();
			bestdist = INF;
			match = KDSearchNN(root, &point);
			obraz[i] = match->key;
		}
		for(i = 0; i < m; i++){
			printf("%d %d %d\n", obraz[i].t[0], obraz[i].t[1], obraz[i].t[2]);
		}
		free(root);
	}
	return 0;
}


