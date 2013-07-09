/* Author Swaroop Butala
   Implementation for wrapper around kdtrees
   */

#include "kdtree.h"

using namespace std;

KDtreeIndex::KDtreeIndex(string &dbname)
{
	indexKDTree = new KD_tree(std::ptr_fun(tac));

}

void KDtreeIndex:: Add_Index(IndexDescPtr input) {
	
	triplet c0(input->kdTreeIndexValues[0],input->kdTreeIndexValues[1], input->kdTreeIndexValues[2], input);
        indexKDTree->insert(c0);
	return;
	
}

void KDtreeIndex :: Delete_Index (string &inp_id){
        return;
}


IndexDescPtr KDtreeIndex::Get_Index (float x, float y, float z) {


}

vector <IndexDescPtr> KDtreeIndex::Get_Index(float x, float y, float z, float range) {

	triplet target(x,y,z, NULL);
	vector<triplet> found;

	vector<IndexDescPtr> output;

	size_t count = indexKDTree->count_within_range(target, range);

	cout << "counted " << count
                << " nodes within range " << range << " of " << target << ".\n";
      	indexKDTree->find_within_range(target, range, std::back_inserter(found));

      	std::cout << "found   " << found.size() << " nodes within range " << range
                << " of " << target << ":\n";
      	std::vector<triplet>::const_iterator ci = found.begin();
      	for (; ci != found.end(); ++ci)
      	{
      		std::cout << *ci << " ";
		output.push_back(ci->ind);
      	}

      	std::cout << "\n" << std::endl;
	
	return output;

}

