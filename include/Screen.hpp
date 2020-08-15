#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class TreeNode
{
public:
	TreeNode() = default;
	TreeNode(const TreeNode& t);
	explicit TreeNode(const std::string& s);
	virtual ~TreeNode ();

private:
	std::string value;
	int count;
	TreeNode* left;
	TreeNode* right;
};
