#include"core/node_func.h"

using namespace asyncflow::core;

bool NodeInnerFunc::call(Agent* agent)
{
	auto* mgr = agent->GetManager();
	return (mgr->*func)(span<int>{params_});
}

NodeFunc* NodeInnerFunc::CreateInnerFunc(FuncType func, std::vector<int> params)
{
	auto* f = new NodeInnerFunc;
	f->func = func;
	f->params_ = params;
	return f;
}

#ifdef FLOWCHART_DEBUG
bool BreakpointFunc::call(Agent* agent)
{
	//continue to run the original function in the node
	if (continue_flag_)
	{
		continue_flag_ = false;
		return pre_nodefunc_->call(agent);
	}
	auto* mgr = agent->GetManager();
	auto* node = mgr->GetCurrentNode();
	// if the chart is not in debug state, breakpoint can not work
	if (!node->GetChart()->IsDebug())
		return pre_nodefunc_->call(agent);
	else
	{
		// add the node to the async manager
		node->SetStatus(Node::Running);
		mgr->GetAsyncManager().AddNode(node);
	}
	return true;
}
#endif