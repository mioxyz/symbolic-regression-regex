#include "node.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string_view>
#include <string>
#include <time.h>
#include <vector>

#define put std::cout << " " <<
#define puts std::cout << std::endl <<

template<typename stuff> using vector = std::vector<stuff>;
using string = std::string;
using view = std::string_view;

/* generates:
   1. random regex patterns.
   2. example strings to random regex patterns.

   For the sake of simplicity we are starting out with Basic Regular Expressions,
   since the amount of symbols is limited to:
   ».«, »[]«, »[^]«, »*«, »^« and »$«

   And we are going to make things even simpler by removing »^« and »$« completely (and implicitly
   requiring that each string starts and ends with the anchors, so to say, since
   we want to have exactly one match, and that match being the entire string).
   We ignore groups, multiple matches are allowed, as long as there is one match
   which matches the entire string. If there are e.g. two matches and they
   partition the string (but in sum cover enitre string), this doesn't count.

   web: https://en.wikibooks.org/wiki/Regular_Expressions/Basic_Regular_Expressions

   ".at"       any three-character string like hat, cat or bat
   "[hc]at"    hat and cat
   "[^b]at"    all the matched strings from the regex ".at" except bat
   "^[hc]at"	hat and cat, but only at the beginning of a line
   "[hc]at$"	hat and cat, but only at the end of a line

   "woefij"    random letters
   ".weorij"   the dot, any position

   we need to think about nesting. How do symbols nest into eachother?
   we have leaf symbols such as a though z and . and *, but then
   we also have "[]". In the moment nesting is not that important,
   but it will quickly become so when we add »()« and »{NUMBER}«
   so coming up with a nested structure is maybe better.
   we have one root Node called caret-dollar aka »^STUFF$« aka root.
   I guess for now we can just choose a Node at random, at any
   position in the tree and just add shit there at random.
   However, certain Nodes require elements, such as »[]«
*/

Node* selectRandomNode(vector<Node*> nodes) {
   return nodes[rand() % nodes.size()];
}


#define ALL_SYMBOLS_SIZE 5
const std::array<char, ALL_SYMBOLS_SIZE> allSymbols = {'.', '*', 'a', 'b', 'c'};
char randomLeafSymbol() {
   return allSymbols[rand() % ALL_SYMBOLS_SIZE];
}

void addRandomTerminalToNode(Node* node) {
   puts "+++addRandomTerminalToNode";
   put " for node(" << node->id << ")";
      auto dump = Node::randomNonRootType();
      switch(dump) {
         case Node::Type::Const: {
            puts "case Const: adding random leaf to any node.";
            auto dump2 = randomLeafSymbol();
            puts "dump2: " << dump2;
            node->addTerminal(dump2);
         } break;
         case Node::Type::Bracket:
            puts "case square brackets";
            addRandomTerminalToNode(
               node->addTerminal(Node::Type::Bracket)
            );
            break;
         case Node::Type::Root:
            throw std::logic_error("not implemented. And you probably don't want to.");
         default:
            puts "dump: " << dump;
            puts "dump: " << dump;
            puts "dump: " << dump;
            puts "dump: " << dump;

            throw std::invalid_argument("impossible");
      }
}

// DELETE_ME
void debugStuff(Node* any) {
   if(Node::Type::Const == any->type) {
      puts "   random node's leafSymbol: »" << any->leafSymbol << "«";
   }else if (Node::Type::Bracket) {
      puts "   random node is SquareBrackets with " << any->terminals.size() << " terminals";
   }
}

auto generatePatterns(int symbolCount) -> string {
   string pattern;
   auto root = Node();
   vector<Node*> nodes = vector<Node*>();
   nodes.push_back(&root);

   for(;symbolCount > 0; --symbolCount) {
      auto any = nodes[rand() % nodes.size()];
      puts "random node id: " << any->id;
      debugStuff(any);
      addRandomTerminalToNode(any);

      puts "---";
   }

   // Node::printLayers(root.buildLayers());

   // root.consolidateBrackets();
   puts root.stringify();
   root.consolidateBrackets();
   root.consolidateBrackets();
   root.consolidateBrackets();
   puts root.stringify();
   return root.stringify();
}

// containers have to be handled recursively. Doesn't make that much sense for brackets,
// since having something like »[a*]« <=> »[a]«, but then we would need to forbid asterisk and dot
// characters (operators) in brackets all-together. Future containers do require us to recurse,
// such as »(abc(xyz)*)*«.
auto sampleGenerator(Node& root) -> string {
   std::stringstream ss;
   for(const auto& terminal : root.terminals) {
      if(Node::Type::Asterisk == terminal->type) {
         
      }
   }

}


auto main() -> int {
   puts "+++main";
   auto seed = time(NULL);
   puts "   >>> seed: " << seed;
   srand(seed);
   //srand(1673774898);

   puts "RESULT:";
   vector<string> patterns = {
      generatePatterns(3),
      generatePatterns(4),
      generatePatterns(5),
      generatePatterns(6),
      generatePatterns(7),
      generatePatterns(7)
   };

   puts "patterns:";
   for(auto& pattern : patterns) {
      puts pattern;
   }
   puts "---main";
}

/*
   Nested bracktes don't make sense.
   »^[[a]]*$«
   is interpreted as
   »[\[a]\]*« 
   where »\[« and »\]« are just normal symbols.

   This simpler form of regex isn't really nested at all,
   and can be "flatly" consumed.
   We have only the following cases:
   1. »...a...« const symbol
   2. »...a*...« repeat const symbol as often as we want.
   3. »...[abc]*...« repeat any symbol in list as often as we want.
   we only have 1 real operator »*«.
   
   consts really are just »[a]« with »a« const. are eq. »...a...« <=> »...[a]...« obviously.
   so we are only really dealing with:
   »[a][bc][d]*[ef]*.« -> »{ Brackets("a"), brackets({b, c}), brackets({d}), *, brackets({e,f}) }«
   find first (from left) non-const and transform this into const (repeat):
   »a[bc][d]*[ef]*«
   »ab[d]*[ef]*«
   »ab[d][d][d][ef]*« notice that we need to process * before we process brackets!
   
   »abd*[ef]*«
   »abdddd[ef]*«
   »abddddf*«
   »abddddfffff«
   ---
   we can go even further and require certain symbols to be processed first:
   »a[bc][d]*[ef]*«
   »a[bc][d][d][d][ef][ef][ef]«
   »ab[d][d][d][ef][ef][ef]«
   »abd[d][ef][ef][ef]«
   »abdd[ef][ef][ef]«
   »abddf[ef][ef]«
   »abddff[ef]«
   »abddffe«

   what about: »a.*b«, then * has prio:
   »a...b«
   »aXYZb«
   
   so ranking is: »*«, ».«, »[ ]«, »a«
*/
