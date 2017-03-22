# ece574-VC
This is ece574 Version Control.

HW#5 Finally Graded
Had right idea, but often result not an *exact* match. Often due to edge conditions.
Does that matter? Approximate computing?
• HW#6 also Finally graded
◦ Comment your code! Even if it’s just a few new lines,
say what it is supposed to be doing. Parallelism is
never trivial.
◦ Have to put “parallel” either in separate directive, or
in sections.
◦ Also time measurement outside parallel area (time in
each section is the same with or without threads, the difference is they can happen simultaneously). i.e. be sure to measure wall clock, not user, time
◦ Don’t nest parallel! remove sections stuff for fine.
◦ Also, does it makes sense to parallelize the most inner
loop of 3?
◦ Also what if you mark variables private that shouldn’t
be? scope!
◦ Also if have sum marked private in inner loop, need
to make sure it somehow gets added on the outer
(reduction).
◦ Be careful with bracket placement. Don’t need one for
a for, for example.
◦ Also, remember as soon as you do parallel everything
in the brackets runs on X threads. So if you parallel, have loops, then a for... those outer loops are each running X times so you’re calculating everything X times over. This isn’t a race condition because we don’t modify the inputs so it doesn’t matter how many times we calc each output.
