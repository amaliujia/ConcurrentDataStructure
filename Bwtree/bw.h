//
// Created by 王 瑞 on 16-6-10.
//

#ifndef BWTREE_BW_H
#define BWTREE_BW_H

#include <functional>

namespace BWTree {

// Currently this bwtree is a Set. So only support unique keys.
template <typename KeyType,
          typename ValueType,
          typename KeyComparator = std::less<KeyType>,
          typename KeyEqualityChecker = std::equal_to<KeyType>,
          // typename ValueEqualityChecker = std::equal_to<ValueType>
        >
class BWTree {


class TreeBaseNode {

};

class InnerNode : public TreeBaseNode {

};

class LeafNode : public TreeBaseNode {

};

};

}  // namespace BWTree

#endif //BWTREE_BW_H
