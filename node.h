#pragma once
#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <list>
#include <map>

#define put std::cout << " " <<
#define puts std::cout << std::endl <<

template<typename stuff> using vector = std::vector<stuff>;
template<typename stuff> using list = std::list<stuff>;

using string = std::string;
using view = std::string_view;


class Node {
 public:
 //   so ranking is: »ROOT«, »*«, ».«, »[ ]«, »a«
   enum Type {
      Root = 0,
      Asterisk = 1,
      Dot = 2,
      Bracket = 3,
      Const = 255,
   };

   Type type;
   Node* origin;
   list<Node*> terminals;
   int id;
   char leafSymbol = 'a';
   bool markedForDeletion = false;

   Node(Type type = Type::Root, Node* origin = nullptr, int id = 0);

   Node(char leafSymbol, Node* origin = nullptr, int id = 0);

   ~Node();

   static auto randomNonRootType() -> int;

   auto addTerminal(Type) -> Node*;

   auto addTerminal(char leafSymbol) -> Node*;

   auto removeTerminal(Node*) -> bool;

   auto stringify() -> string;

   /*
      I sort of doubt that this is tail-recursive, but it would
      be interesting to see if it is by looking at assembly.
   */
   auto register_() -> int;

   //removes unnecessary brackets
   auto reduceBrackets() -> bool;

   auto consolidateBrackets() -> void;

   auto buildLayers() -> std::map<int, vector<Node*>>;

   static void printLayers(std::map<int, vector<Node*>> layers);

private:
   int idCount = 0;

   static void buildLayersRecursion(Node* node, std::map<int, vector<Node*>>& layers, int layerNumber);


};
