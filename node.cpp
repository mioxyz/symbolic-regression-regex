#include "node.h"
#include <algorithm>
#include <array>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <map>

Node::Node(Type type, Node* origin, int id)
: type(type), origin(origin), id(id)
{ }

Node::Node(char leafSymbol, Node* origin, int id)
: leafSymbol(leafSymbol), /*type(Type::Const),*/ origin(origin), id(id)
{
   type = Type::Const;
}

Node::~Node() {
   for(auto& terminal : terminals) {
      delete terminal;
   }
}

auto Node::randomNonRootType() -> Node::Type {
   puts "+++randomNonRootType";
   return allNonRootTypes[rand() % ALL_NON_ROOT_TYPES_SIZE];
}

auto Node::addTerminal(Node::Type type) -> Node* {
   puts "addTerminal" << type;
   // if(origin == nullptr) throw std::logic_error("origin is nullptr!"); //TODO deleteme

   int newId = register_();
   terminals.push_back(new Node(type, this, newId));
   return terminals.back();
}

auto Node::addTerminal(char leafSymbol) -> Node* {
   puts "addTerminal(byLeafSymbol): »" << leafSymbol << "«";
   // if(origin == nullptr) throw std::logic_error("origin is nullptr!"); //TODO deleteme
   int newId = register_();
   terminals.push_back(new Node(leafSymbol, this, newId));
   return terminals.back();
}

auto Node::removeTerminal(Node* terminal) -> bool {
   auto maybe = std::ranges::find_if(terminals, [terminal](Node* adjacent) {
      return adjacent->id == terminal->id;
   });
   if(terminals.end() == maybe) return false;
   terminals.erase(maybe);
   return true;
}

auto Node::stringify() -> string {
   std::stringstream ss;
   switch(type)
   {
      case Node::Type::Const:
         ss << leafSymbol;
         return ss.str();
         break;
      case Node::Type::Root:
         ss << "^";
         for(auto& terminal : terminals) {
            ss << terminal->stringify();
         }
         ss << "$";
         return ss.str();
         break;
      case Node::Type::Bracket:
         ss << "[";
         for(auto& terminal : terminals) {
            ss << terminal->stringify();
         }
         ss << "]";
         return ss.str();
   }
   return "";
}

/*
   I sort of doubt that this is tail-recursive, but it would
   be interesting to see if it is by looking at assembly.
*/
auto Node::register_() -> int {
   // if(Type::Root == type) {
   //    idCount++;
   //    return idCount;
   // }else{
   //    puts "(was leaf!)register_ before recursion...";
   //    auto dump = origin->register_();
   //    puts "(was leaf!)register_ after recursion... dump: " << dump;
   //    return dump;
   // }
   return (Node::Type::Root == type) ? ++idCount
                                 : origin->register_();
}


//removes unnecessary brackets
auto Node::reduceBrackets() -> bool {
   if(Node::Type::Bracket == type
   && terminals.size() == 1
   && Node::Type::Bracket == terminals.back()->type
   ) {
      auto redundant = std::move(terminals.back()); // DANGER!
      // node->terminals.erase(node->terminals.begin() + 1);
      terminals.clear(); // DANGER!
      // redundant->reduceBrackets();
      origin->terminals.push_back(redundant);
      origin->removeTerminal(this);
      return true;
   } else {
      int counter = 0;
      /*
         for(auto& terminal : terminals) {
            puts "terminal-counter: " << counter;
            try {
               puts terminal->id;
            } catch(...) {
               puts "terminal->id unreadable!";
            }
            terminal->reduceBrackets();
            counter++;
         }
      */

      for(bool bleh = true; bleh;) {
         bleh = false;
         for(list<Node*>::iterator itr = terminals.begin(); itr != terminals.end();) {
            puts "counter: " << counter;
            bleh = (*itr)->reduceBrackets();
            ++counter;
         }
      }

      return false;
   }
}

auto Node::consolidateBrackets() -> void {
   if(Node::Type::Root != type) {
      throw std::logic_error("bleh");
   }
   auto layers = buildLayers();
   int layerCount = 0;
   for(auto& elem : layers) if(layerCount < elem.first) layerCount = elem.first;
   for(int k = layerCount; k >= 0; --k) {
      for(auto& node : layers[k]) {
         //move terminals up to origin, if both node and origin are Brackets
         if(node->type == Node::Type::Bracket && node->origin->type == Node::Type::Bracket) {
            for(auto& terminal : node->terminals) {
               node->origin->terminals.push_back(terminal);
            }
            node->origin->removeTerminal(node);
         }
      }
   }

}



void Node::buildLayersRecursion(Node* node, std::map<int, vector<Node*>>& layers, int layerNumber) {
   if(layers.find(layerNumber) == layers.end())
      layers[layerNumber] = vector<Node*>();

   layers[layerNumber].push_back(node);

   layerNumber++;
   for(auto& terminal : node->terminals) {
      buildLayersRecursion(terminal, layers, layerNumber);
   }
}

std::map<int, vector<Node*>> Node::buildLayers() {
   auto layers = std::map<int, vector<Node*>>();
   buildLayersRecursion(this, layers, 0);
   return layers;
}

void Node::printLayers(std::map<int, vector<Node*>> layers) {
   for(auto& pr : layers) {
      std::cout << std::endl << "layer[" << pr.first << "]: ";
      for(auto& node : pr.second) {
         std::cout << "node[" << node->id << "], ";
      }
   }
}
