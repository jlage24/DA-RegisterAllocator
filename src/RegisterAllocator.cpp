/**
 * @file RegisterAllocator.cpp
 * @brief Implementation of the main allocation pipeline orchestrator.
 *
 * This file ties all components together. It is a shared responsibility
 * but can be implemented by either team member once the other pieces are ready.
 *
 * ============================================================
 * PIPELINE
 * ============================================================
 *   INPUT: rangesByVar (from Parser) + config (from Parser)
 *
 *   Step 1 [WebBuilder]              : LiveRanges -> Webs
 *   Step 2 [InterferenceGraphBuilder]: Webs -> Graph
 *   Step 3 [Algorithm]               : Graph -> Register assignments
 *   Step 4 [Result]                  : Package result for OutputWriter
 *
 * ============================================================
 * ALGORITHM DISPATCH
 * ============================================================
 *   config.algorithm == BASIC     -> GraphColoringAlgorithm::findMinColors()
 *   config.algorithm == SPILLING  -> SpillingAlgorithm::allocate(graph, N, K)
 *   config.algorithm == SPLITTING -> SplittingAlgorithm::allocate(graph, webs, N, K)
 *   config.algorithm == FREE      -> FreeAlgorithm::allocate(graph, webs, N)
 *
 * ============================================================
 * RESULT PACKAGING
 * ============================================================
 *   - result.webs          = the (possibly modified) web list
 *   - result.registersUsed = number of distinct colors used (0 if infeasible)
 *   - result.feasible      = true if all webs got a register (none spilled forcefully)
 *   Each web's assignedRegister field is set by the algorithm:
 *     >= 0 : register number
 *     -1   : spilled to memory
 */

#include "RegisterAllocator.h"
#include "WebBuilder.h"
#include "InterferenceGraphBuilder.h"
#include "GraphColoringAlgorithm.h"
#include "SpillingAlgorithm.h"
#include "SplittingAlgorithm.h"
#include "FreeAlgorithm.h"

// -----------------------------------------------------------------------------
// AllocationResult RegisterAllocator::allocate(...)
// -----------------------------------------------------------------------------
// Runs the complete pipeline and returns the result.
//
// STEPS:
//   1. Call WebBuilder::buildWebs(rangesByVar) -> webs
//   2. Call InterferenceGraphBuilder::build(webs) -> graph
//   3. Switch on config.algorithm:
//      BASIC:    call GraphColoringAlgorithm::colorGraph(graph, config.numRegisters)
//      SPILLING: call SpillingAlgorithm::allocate(graph, N, K)
//      SPLITTING:call SplittingAlgorithm::allocate(graph, webs, N, K)
//      FREE:     call FreeAlgorithm::allocate(graph, webs, N)
//   4. After the algorithm runs, scan webs:
//      - Count distinct register colors used (values >= 0).
//      - If any web has assignedRegister == -1 and the algorithm was BASIC,
//        the allocation is infeasible (result.feasible = false).
//   5. Fill and return the AllocationResult.
//
// TODO: implement step by step
AllocationResult RegisterAllocator::allocate(
    const std::map<std::string, std::vector<LiveRange>>& rangesByVar,
    const AllocConfig& config)
{
    AllocationResult result;
    result.feasible = false;
    result.registersUsed = 0;

    // TODO: Step 1 - build webs
    // TODO: Step 2 - build graph
    // TODO: Step 3 - dispatch to algorithm
    // TODO: Step 4 - package result

    return result;
}
