// The MIT License (MIT)
// Copyright (c) 2014 Matthew Klingensmith and Ivan Dryanovski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <open_chisel/Chunk.h>

namespace chisel
{

    Chunk::Chunk()
    {
        // TODO Auto-generated constructor stub

    }

    Chunk::Chunk(const ChunkID id, const Eigen::Vector3i& nv, float r, bool useColor) :
            ID(id), numVoxels(nv), voxelResolutionMeters(r)
    {
        AllocateDistVoxels();

        if(useColor)
        {
            AllocateColorVoxels();
        }

        origin = Vec3(numVoxels(0) * ID(0) * voxelResolutionMeters, numVoxels(1) * ID(1) * voxelResolutionMeters, numVoxels(2) * ID(2) * voxelResolutionMeters);
    }

    Chunk::~Chunk()
    {

    }

    void Chunk::AllocateDistVoxels()
    {
        int totalNum = GetTotalNumVoxels();
        voxels.clear();
        voxels.resize(totalNum, DistVoxel());
    }

    void Chunk::AllocateColorVoxels()
    {
        int totalNum = GetTotalNumVoxels();
        colors.clear();
        colors.resize(totalNum, ColorVoxel());
    }

    AABB Chunk::ComputeBoundingBox()
    {
        Vec3 pos = origin;
        Vec3 size = numVoxels.cast<float>() * voxelResolutionMeters;
        return AABB(pos, pos + size);
    }

    VoxelID Chunk::GetVoxelID(const Vec3& worldPos)
    {
        Vec3 chunkPos = (worldPos - origin) / voxelResolutionMeters;
        return GetVoxelID(std::max(std::min(static_cast<int>(chunkPos(0)), numVoxels(0)), 0),
                          std::max(std::min(static_cast<int>(chunkPos(1)), numVoxels(1)), 0),
                          std::max(std::min(static_cast<int>(chunkPos(2)), numVoxels(2)), 0));
    }

    Vec3 Chunk::GetColorAt(const Vec3& pos)
    {
        if(ComputeBoundingBox().Contains(pos))
        {
            Vec3 chunkPos = (pos - origin) / voxelResolutionMeters;
            int chunkX = static_cast<int>(chunkPos(0));
            int chunkY = static_cast<int>(chunkPos(1));
            int chunkZ = static_cast<int>(chunkPos(2));

            if(IsCoordValid(chunkX, chunkY, chunkZ))
            {
                const ColorVoxel& color = GetColorVoxel(chunkX, chunkY, chunkZ);
                float maxVal = static_cast<float>(std::numeric_limits<uint8_t>::max());
                return Vec3(static_cast<float>(color.GetRed()) / maxVal, static_cast<float>(color.GetGreen()) / maxVal, static_cast<float>(color.GetBlue()) / maxVal);
            }
        }

        return Vec3::Zero();
    }


} // namespace chisel 
