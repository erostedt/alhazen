#pragma once

#include "box.hpp"

#include <algorithm>
#include <vector>

#include "object.hpp"
#include "types.hpp"

struct BVHNode
{
    Box BoundingBox;
    u32 Left;
    u32 Right;

    inline bool IsLeaf() const noexcept
    {
        return Left == Right;
    }

    inline u32 ObjectIndex() const noexcept
    {
        return Left;
    }
};

struct BVH
{
    std::vector<BVHNode> Nodes;
    std::vector<Object> Objects;
};

static u32 BuildBVH(BVH &bvh, u32 start, u32 end)
{
    std::vector<BVHNode> &nodes = bvh.Nodes;

    if (start + 1 == end)
    {
        u32 node_index = (u32)nodes.size();
        Box &box = bvh.Objects[start].BoundingBox;
        nodes.emplace_back(box, start, start);
        return node_index;
    }

    Box box = Box::Empty();
    for (u32 i = start; i < end; ++i)
    {
        box = Expand(box, bvh.Objects[i].BoundingBox);
    }

    switch (box.LongestAxis())
    {
    case 0: {
        std::sort(bvh.Objects.begin() + start, bvh.Objects.begin() + end, [](const Object &o1, const Object &o2) {
            return o1.BoundingBox.XAxis.Midpoint() < o2.BoundingBox.XAxis.Midpoint();
        });
        break;
    }
    case 1: {
        std::sort(bvh.Objects.begin() + start, bvh.Objects.begin() + end, [](const Object &o1, const Object &o2) {
            return o1.BoundingBox.YAxis.Midpoint() < o2.BoundingBox.YAxis.Midpoint();
        });
        break;
    }
    case 2: {
        std::sort(bvh.Objects.begin() + start, bvh.Objects.begin() + end, [](const Object &o1, const Object &o2) {
            return o1.BoundingBox.ZAxis.Midpoint() < o2.BoundingBox.ZAxis.Midpoint();
        });
        break;
    }
    }

    u32 mid = start + (end - start) / 2;
    u32 node_index = (u32)nodes.size();
    BVHNode &node = nodes.emplace_back(box, 0, 0);
    node.Left = BuildBVH(bvh, start, mid);
    node.Right = BuildBVH(bvh, mid, end);
    return node_index;
}

inline BVH CreateBVH(std::vector<Object> &&objects)
{
    BVH bvh;
    bvh.Objects = std::move(objects);
    sz capacity = bvh.Objects.empty() ? 0 : 2 * bvh.Objects.size() - 1;
    bvh.Nodes.reserve(capacity);

    BuildBVH(bvh, 0, (u32)bvh.Objects.size());
    return bvh;
}
