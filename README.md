
# Solution to C++ Technical Test

This is my solution to your test that checks if a binary tree contains a cycle. We can either perform a breadth-first search or depth-first search traversal of the tree, keeping track of the parent node for each visited node. This will allow to identify any back edges that indicate the presence of a cycle.
 
In the solution named "simple," I use breadth-first search to solve the task. However, I find the code a bit cluttered within this solution, especially in the `has_circle` function where we have inline code for breadth-first search. Nevertheless, for the sake of simplicity, I have decided to include it.

In the solution named "elaborated," I believe that the code appears to be more appropriate for real projects. Additionally, the solution is more elegant as we detect the circle by checking for the presence of a back edge. The code becomes less cluttered because each method is more aligned with a single intention.
