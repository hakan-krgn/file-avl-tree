#pragma once

#include <string>

#define NODE_SIZE (int) sizeof(Node)

struct Node {
    int key;
    int value;

    int index = -1;
    int height = 0;
    int left_index = -1;
    int right_index = -1;

    void print() {
        printf("Node={key=%d, value=%d, index=%d, height=%d, left_index=%d, right_index=%d}\n",
               key, value, index, height, left_index, right_index);
    }
};

class FAVLTree {
public:
    Node head{};
    FILE *file{};
    std::string path;
    int current_index{};

public:
    FAVLTree() = default;

    explicit FAVLTree(const std::string &path);


    Node read(int index);

    Node update(Node node);

    Node save(int key, int value);

    Node successor(Node node);

    Node predecessor(Node node);

    Node left_rotate(Node &node);

    Node right_rotate(Node &node);



    Node search(int key);

    Node search(Node &node, int key);


    Node insert(int key, int value);

    Node insert(Node &node, int key, int value);


    Node remove(int key);

    Node remove(Node &node, int key);
};
