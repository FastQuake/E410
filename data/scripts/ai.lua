local nodes = {}
AIManager = {}

function AIManager.addNode(id,x,y,z)
	local node = {}
	node.x = x
	node.y = y
	node.z = z
	node.id = id
	node.neighbors = {}
	table.insert(nodes,node)
end

function AIManager.addNeighbor(id1,id2)
	local node = nodes[id1]
	table.insert(node.neighbors,id2)
end

function AIManager.getNode(id)
	return nodes[id]
end
