mkdir -p bin; clang++ -std=c++20 training_dataset_generator.cpp node.cpp -g -o bin/training_dataset_generator && bin/training_dataset_generator

We want a simple, yet data-set-confined regex pattern that matches a group of strings.
We might be able to make the following conclusions of the string-set we want to classify:

   i. examples which must be included OR a group of examples for which the algorithm has to include a pre-defined minimum percentage.
  ii. the estimated actual size of the set.
 iii. anti examples which must not be included.

The algorithm can first be developed to only include i. This alone is a challenge. It might be wise to start out with a subset of 
regex symbols and not all of them, just some fundamentals, such as »[a-z]« and »[0-9]« or whatever.
We must not forget the linear nature of the data which we are looking at, in particular that the string might be describable by local patterns which are disjoint from eachother. In other words, substrings at the beginning of the string might not correlate at all to substrings at the end, for example.

If we pit two regex patterns against eachother, how do we determine which one is "better"?
First, we want only regex patterns which only match the string once and completely. So ».« would only be a valid pattern for any 
single char string, ergo, if we are dealing with ASCII with 128 characters, ».« will match 128 strings.
consider patterns such as ».\*« which will match any string. This is the "worst" pattern for almost all string-sets, since it doesn't exclude string patterns. The exception is the set of all strings. For this string-set the pattern ».\*« is optimal, since it is the shortest possible pattern which describes all strings in the set. Proving this is trivial, since there are very few patterns which require less symbols than ».\*«, listing these and showing that they do not match all strings in the string-set of all strings proves this.
The second criteria is the size of the pattern, which is the amount of symbols in the string which represents the pattern. We could 
say that groups such as »[a-z]« are a single symbol, but I'm not sure if this is a good idea. We probably should try out a few of these possibilities and see how it affects results.
Coming up with different size metrics is a good idea.

There is a problem with symbolic regression with regex patterns as opposed to symbolic regression on smooth curves: there is no incrementally correct answer, so to say. I guess with looser restrictions on which strings need to be matched, yes. But getting any string to match is a challenge in itself, sorta. I guess we could develop a seed algorithm, which just finds regex patterns which match a single given string... but this is also a bit weird because: let str := »xyz« then the pattern »[x][y][z]« is a valid pattern. I guess this isn't terrible, since the string itself is a good description of itself. That is right: strings sort of can be seen as atomic patterns of themselves, since they describe exactly only themselves and nothing else. Indeed this isn't true in general for regex patterns, since there are probably infinitely many patterns which describe any single string, and only that string. But there is still some truth in this, since if we look at the set of possible regex patterns that describe the string perfectly, there will be a small set of super-descriptors.
Look at the string: »abcabcabcabcabcabcabcabc« and the matching pattern »(abc){8}«. The pattern is obviously much more compact than the string, and then calling the string "atomic" in this sense might feel a bit wrong.. but I think that is a mistake. Especially if the repeated pattern of »abc« is much longer, the length becomes a bit meaningless. 
I wanted to write that the length of the string might be a good cutoff value at which we can say that smaller patterns are super-descriptors, but this is obviously wrong...

I have an idea how we can generate training datasets: we generate random regex patterns and then generate possible strings therefrom.
The regex-pattern might not be ideal, but we would be able to generate something like »abcabcabc« and I see value in this, for coming up with a general solution.

I guess writing this training dataset generator might be a good starting point and will give me a bit time to think more about the problem.

---
important exception: any pattern is allowed to match the null-string aka empty string: "" or new line stirng "\n". This is because many good patterns will match the empty string.
I guess we can always add the empty string to the set of example strings instead of having this hard rule.
It is worth thinking about the meaning of this once we get deeper into the weeds.

