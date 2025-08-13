import pytest
from typing import List, Dict, Set
from collections import defaultdict
from dataclasses import dataclass
import heapq


@dataclass(order=True, frozen=True)
class Path:  # Single edge is a Path as well
    delay_time: int
    node_to: int


class Solution:
    def networkDelayTime(
        self, times: List[List[int]], num_nodes: int, target: int
    ) -> int:
        adj_map: Dict[int, Set[Path]] = defaultdict(set)
        for [node_from, node_to, delay_time] in times:
            adj_map[node_from].add(Path(delay_time, node_to))
        seen: Set[int] = set()
        heap: List[Path] = []
        heapq.heapify(heap)
        heapq.heappush(heap, Path(0, target))
        res: int = 0
        while heap:
            cur = heapq.heappop(heap)
            cur_delay, cur_node = cur.delay_time, cur.node_to
            if cur_node in seen:
                continue
            res = max(res, cur_delay)
            for edge in adj_map[cur_node]:
                edge_delay, edge_node = edge.delay_time, edge.node_to
                if edge_node not in seen:
                    heapq.heappush(heap, Path(cur_delay + edge_delay, edge_node))
            seen.add(cur_node)
        for n in range(1, num_nodes + 1):
            if n not in seen:
                return -1
        return res


@pytest.mark.parametrize(
    "times, n, k, expected",
    [
        # Simple example from the problem description
        ([[2, 1, 1], [2, 3, 1], [3, 4, 1]], 4, 2, 2),
        # Single node, no edges (delay is zero because only one node)
        ([], 1, 1, 0),
        # Disconnected graph: node 4 cannot be reached from node 1
        ([[1, 2, 1], [2, 3, 1]], 4, 1, -1),
        # All nodes reachable with different weights
        ([[1, 2, 5], [1, 3, 2], [3, 2, 1], [2, 4, 1]], 4, 1, 4),
        # Node K itself has no outgoing edges and n > 1 (impossible to reach others)
        ([], 2, 1, -1),
        # Larger graph with multiple paths
        ([[1, 2, 2], [1, 3, 4], [2, 3, 1], [3, 4, 1], [2, 4, 7], [4, 5, 3]], 5, 1, 7),
        # Multiple edges from same node to same target with different weights
        ([[1, 2, 1], [1, 2, 2], [2, 3, 1]], 3, 1, 2),
        # Cycle in the graph but all reachable
        ([[1, 2, 1], [2, 3, 2], [3, 1, 4]], 3, 1, 3),
        # Large weights
        ([[1, 2, 100000], [2, 3, 100000]], 3, 1, 200000),
    ],
)
def test_network_delay_time(times, n, k, expected):
    assert Solution().networkDelayTime(times, n, k) == expected


def test_unreachable_node():
    # Node 2 cannot be reached from node 1
    times = [[1, 3, 1]]
    n = 3
    k = 1
    assert Solution().networkDelayTime(times, n, k) == -1
