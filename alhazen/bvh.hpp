#pragma once

#include "box.hpp"
#include "object.hpp"
#include "types.hpp"
#include <algorithm>
#include <numeric>
#include <span>
#include <vector>

struct BVHNode
{
    Box BoundingBox;
    i32 ObjectIndex = -1;
    u32 Left;
    u32 Right;

    inline bool IsLeaf() const noexcept
    {
        return ObjectIndex >= 0;
    }
};

struct BVH
{
    std::vector<BVHNode> Nodes;
    std::vector<Object> Objects;
};

static u32 BuildBVH(BVH &bvh, std::span<u32> indices)
{
    std::vector<BVHNode> &nodes = bvh.Nodes;

    if (indices.size() == 1)
    {
        u32 object_index = indices.front();
        u32 node_index = (u32)nodes.size();
        Box &box = bvh.Objects[object_index].BoundingBox;
        nodes.emplace_back(box, object_index, 0, 0);
        return node_index;
    }

    Box box = Box::Empty();
    for (const u32 i : indices)
    {
        box = Expand(box, bvh.Objects[i].BoundingBox);
    }

    switch (box.LongestAxis())
    {
    case 0: {
        std::sort(indices.begin(), indices.end(), [&](u32 i, u32 j) {
            return bvh.Objects[i].BoundingBox.XAxis.Midpoint() < bvh.Objects[j].BoundingBox.XAxis.Midpoint();
        });
        break;
    }
    case 1: {
        std::sort(indices.begin(), indices.end(), [&](u32 i, u32 j) {
            return bvh.Objects[i].BoundingBox.YAxis.Midpoint() < bvh.Objects[j].BoundingBox.YAxis.Midpoint();
        });
        break;
    }
    case 2: {
        std::sort(indices.begin(), indices.end(), [&](u32 i, u32 j) {
            return bvh.Objects[i].BoundingBox.ZAxis.Midpoint() < bvh.Objects[j].BoundingBox.ZAxis.Midpoint();
        });
        break;
    }
    }

    sz mid = indices.size() / 2;
    std::span<u32> left_indices = indices.subspan(0, mid);
    std::span<u32> right_indices = indices.subspan(mid);

    u32 node_index = (u32)nodes.size();
    BVHNode &node = nodes.emplace_back(box, -1, 0, 0);
    node.Left = BuildBVH(bvh, left_indices);
    node.Right = BuildBVH(bvh, right_indices);
    return node_index;
}

inline BVH CreateBVH(std::vector<Object> &&objects)
{
    BVH bvh;
    bvh.Objects = std::move(objects);
    sz capacity = bvh.Objects.empty() ? 0 : 2 * bvh.Objects.size() - 1;
    bvh.Nodes.reserve(capacity);

    std::vector<u32> indices(bvh.Objects.size());
    std::iota(indices.begin(), indices.end(), 0);
    BuildBVH(bvh, indices);
    return bvh;
}
