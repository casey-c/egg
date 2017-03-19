#include "grid.h"
#include <QStack>
#include <QQueue>
#include <QDebug>

/*
 * Constructor
 */
Grid::Grid(TreeState* t) :
    tree(t)
{
    /// First, let's BFS to make all the regions ///

    // Root
    root = new GridRegion(tree->getRoot());

    // Set up the initial queues
    QQueue<TreeNode*> queue;
    QQueue<GridRegion*> parentQueue;

    for (TreeNode* child : tree->getRoot()->getChildren())
    {
        queue.enqueue(child);
        parentQueue.enqueue(root);
    }

    // Work through the queues
    while (!queue.isEmpty())
    {
        // Determine what exists so far
        TreeNode* current = queue.dequeue();
        GridRegion* parent = parentQueue.dequeue();

        // Make the new region
        GridRegion* region = parent->addChildRegion(current);

        // Append the children to the queue
        for (TreeNode* child : current->getChildren())
        {
            queue.enqueue(child);
            parentQueue.enqueue(region);
        }
    }

    /// At this point, all the regions are allocated, but lacking detail. To
    /// fill them in with the proper details, we need to construct them from the
    /// bottom up: i.e. all children before their parent, as the parent needs
    /// each and every child's own data before it can build its own grid.
    ///
    /// DFS can easily build a bottom-up ordering, so we use that to our
    /// advantage.

    // The stack starts off with just the root
    QStack<GridRegion*> stack;
    stack.push(root);

    // The ordering (bottom up)
    QList<GridRegion*> order;

    // Work through the stack
    while (!stack.isEmpty())
    {
        GridRegion* region = stack.pop();
        order.prepend(region);

        // Push the kids
        for (GridRegion* child : region->getChildren())
            stack.push(child);
    }

    /// Now that we've allocated the regions and ordered them correctly, all
    /// that remains is to actually build their grids.

    for (GridRegion* region : order)
        region->buildGrid();
}

QString Grid::toPlaintext()
{
    QString result = "<pre>";
    result += root->toString();
    result += "</pre>";
    qDebug() << result;
    return result;
}
