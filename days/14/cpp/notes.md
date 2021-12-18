Awesome! Now **this** is Advent of Code!

Initially I cursed myself for wishing for a harder task than the previous days, but this one turned out to be an absolute joyride instead. It has all the best elements.

Part 1 is easy with any naive approach. The difficulty is in parsing the input and in choosing data structures that don't require ugly workarounds for in-place mutation. And it *smells* like incoming exponential growth.

Sure enough, part 2 comes crashing in with numbers infeasible for brute-force. But the rules are delicately designed to allow for very easy iterative computation (my fears of having to detect cycles in rule-expansion were unfounded). I did have to rewrite my solution pretty much from scratch and could not reuse much of part 1, but I'll tell myself that this is a good thing because I did not over-engineer part 1 when I didn't yet need to.
