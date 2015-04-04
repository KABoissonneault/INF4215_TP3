#pragma once

#include <memory>

namespace INF4215_TP3
{
    namespace Utility
    {
        class DisjointSet
        {
        private:
            std::unique_ptr<size_t[]> m_aGroups;
            std::unique_ptr<size_t[]> m_aRanks;
            size_t m_nSetCount;

        public:
            // Create an empty union find data structure with N isolated sets
            DisjointSet(size_t nSize)
                : m_aGroups(new size_t[nSize]),
                m_aRanks(new size_t[nSize]),
                m_nSetCount(nSize)
            {
                for(size_t i = 0; i < nSize; ++i)
                {
                    m_aGroups[i] = i;
                    m_aRanks[i] = 1;
                }
            }

            // Return the id of component corresponding to object p.
            size_t find(size_t p)
            {
                size_t root = p;

                // Find the root
                while (root != m_aGroups[root])
                {
                    root = m_aGroups[root];
                }

                // Flatten hierarchy
                while (p != root)
                {
                    size_t newp = m_aGroups[p];
                    m_aGroups[p] = root;
                    p = newp;
                }

                return root;
            }

            // Replace sets containing x and y with their union.
            void merge(int x, int y)
            {
                const size_t i = find(x);
                const size_t j = find(y);
                if (i == j) return;

                // make smaller root point to larger one
                if (m_aRanks[i] < m_aRanks[j])
                {
                    m_aGroups[i] = j;
                    m_aRanks[j] += m_aRanks[i];
                }
                else
                {
                    m_aGroups[j] = i;
                    m_aRanks[i] += m_aRanks[j];
                }

                // We have merged two sets. There are now less sets
                --m_nSetCount;
            }
            // Are objects x and y in the same set?
            bool connected(int x, int y)
            {
                return find(x) == find(y);
            }
            // Return the number of disjoint sets.
            size_t count()
            {
                return m_nSetCount;
            }
        };
    }
}
