#include "favl_tree.h"

FAVLTree::FAVLTree(const std::string &path) {
    this->path = path;
    this->head = Node();
    this->file = fopen(path.data(), "w+");
    this->current_index = 0;
}


Node FAVLTree::read(int index) {
    if (index == -1) return {};

    Node node{};
    fseek(this->file, index * NODE_SIZE, SEEK_SET);
    fread(&node, NODE_SIZE, 1, this->file);
    return node;
}

Node FAVLTree::update(Node node) {
    if (node.index == -1) return {};

    fseek(this->file, node.index * NODE_SIZE, SEEK_SET);
    fwrite(&node, NODE_SIZE, 1, this->file);
    fflush(this->file);
    return node;
}

Node FAVLTree::save(int key, int value) {
    return this->update(Node(key, value, this->current_index++, 1, -1, -1));
}

Node FAVLTree::successor(Node node) {
    if (node.index == -1) return {};

    Node right_node = this->read(node.right_index);
    while (right_node.index != -1) {
        node = right_node;
        right_node = this->read(right_node.right_index);
    }

    return node;
}

Node FAVLTree::predecessor(Node node) {
    if (node.index == -1) return {};

    Node left_node = this->read(node.left_index);
    while (left_node.index != -1) {
        node = left_node;
        left_node = this->read(left_node.left_index);
    }

    return node;
}

Node FAVLTree::left_rotate(Node &node) {
    Node right_node = this->read(node.right_index);
    Node right_left_node = this->read(right_node.left_index);

    right_node.left_index = node.index;
    node.right_index = right_left_node.index;

    node.height = std::max(this->read(node.left_index).height, right_left_node.height) + 1;
    right_node.height = std::max(node.height, this->read(right_node.right_index).height) + 1;

    this->update(node);
    return this->update(right_node);
}

Node FAVLTree::right_rotate(Node &node) {
    Node left_node = this->read(node.left_index);
    Node left_right_node = this->read(left_node.right_index);

    left_node.right_index = node.index;
    node.left_index = left_right_node.index;

    node.height = std::max(this->read(node.right_index).height, left_right_node.height) + 1;
    left_node.height = std::max(node.height, this->read(left_node.left_index).height) + 1;

    this->update(node);
    return this->update(left_node);
}



Node FAVLTree::search(int key) {
    return this->search(this->head, key);
}

Node FAVLTree::search(Node &node, int key) {
    if (node.index == -1) return {};

    if (key == node.key) {
        return node;
    } else if (key < node.key) {
        Node left_node = this->read(node.left_index);
        return this->search(left_node, key);
    } else {
        Node right_node = this->read(node.right_index);
        return this->search(right_node, key);
    }
}


Node FAVLTree::insert(int key, int value) {
    return this->head = this->insert(this->head, key, value);
}

Node FAVLTree::insert(Node &node, int key, int value) {
    if (node.index == -1) {
        return this->save(key, value);
    }

    Node left_node = this->read(node.left_index);
    Node right_node = this->read(node.right_index);

    if (key == node.key) {
        return {};
    } else if (key < node.key) {
        left_node = this->insert(left_node, key, value);
        node.left_index = left_node.index;
    } else {
        right_node = this->insert(right_node, key, value);
        node.right_index = right_node.index;
    }

    node.height = std::max(left_node.height, right_node.height) + 1;

    int balance = left_node.height - right_node.height;
    if (balance < -1 && key > right_node.key) {
        return this->left_rotate(node);
    } else if (balance > 1 && key < left_node.key) {
        return this->right_rotate(node);
    } else if (balance < -1 && key < right_node.key) {
        node.right_index = this->right_rotate(right_node).index;
        return this->left_rotate(node);
    } else if (balance > 1 && key > left_node.key) {
        node.left_index = this->left_rotate(left_node).index;
        return this->right_rotate(node);
    }

    return this->update(node);
}


Node FAVLTree::remove(int key) {
    return this->remove(this->head, key);
}

Node FAVLTree::remove(Node &node, int key) {
    if (node.index == -1) return {};

    Node left_node = this->read(node.left_index);
    Node right_node = this->read(node.right_index);

    if (key == node.key) {
        if (node.left_index == -1 && node.right_index == -1) {
            return {};
        } else if (node.left_index == -1) {
            return right_node;
        } else if (node.right_index == -1) {
            return left_node;
        } else {
            if (left_node.height > right_node.height) {
                Node predecessor = this->predecessor(left_node);
                node.key = predecessor.key;
                node.value = predecessor.value;
                node.left_index = this->remove(left_node, predecessor.key).index;
            } else {
                Node successor = this->successor(right_node);
                node.key = successor.key;
                node.value = successor.value;
                node.right_index = this->remove(right_node, successor.key).index;
            }
        }
    } else if (key < node.key) {
        node.left_index = this->remove(left_node, key).index;
    } else {
        node.right_index = this->remove(right_node, key).index;
    }


    node.height = std::max(left_node.height, right_node.height) + 1;

    int balance = left_node.height - right_node.height;
    if (balance < -1 && key > right_node.key) {
        return this->left_rotate(node);
    } else if (balance > 1 && key < left_node.key) {
        return this->right_rotate(node);
    } else if (balance < -1 && key < right_node.key) {
        node.right_index = this->right_rotate(right_node).index;
        return this->left_rotate(node);
    } else if (balance > 1 && key > left_node.key) {
        node.left_index = this->left_rotate(left_node).index;
        return this->right_rotate(node);
    }

    return this->update(node);
}
