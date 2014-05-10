local nodes = {}
local monsters = {}
AIManager = {}

function AIManager.addNode(id,x,y,z)
	local node = {}
	node.pos = Vector.create(x,y,z)
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

function AIManager.getMonsters()
	return monsters
end

function AIManager.addMonster()
	local monster = {}
	monster.model = GO.loadIQM("cube.iqm","monster")
	monster.model:setPos(18,2,5)
	monster.path = {}
	table.insert(monsters,monster)
end
