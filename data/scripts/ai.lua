local nodes = {}
local monsters = {}
AIManager = {}

function AIManager.addNode(id,x,y,z)
	local node = {}
	node.pos = Vector.create(x,y,z)
	node.id = id
	node.neighbors = {}
	node.model = GO.loadIQM("cube.iqm","node"..id)
	node.model:setBoxBody()
	node.model:setVisible(false)
	node.model:setMass(0)
	node.model:setPos(node.pos:get())
	table.insert(nodes,node)
end

function AIManager.addNeighbors(id1,ids)
	local node = nodes[id1]
	for id in pairs(ids) do
		table.insert(node.neighbors,id)
	end
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

function AIManager.findVisibleNode(startPos)
	local closestNode = {}
	local triedNodes = {}
	local out = {}
	local tries = 0
	while true do
		local min = 9999
		if tries > 15 then return nil end
		for k,v in pairs(nodes) do
			local distance = Vector.distance(startPos,v.pos)
			local seenAlready = 0
			for l,w in pairs(triedNodes) do
				if w.id == v.id then
					seenAlready = 1
				end
			end
			if (distance < min and seenAlready ~= 1) then
				min = distance
				closestNode = v
			end
		end
		local dir = closestNode.pos-startPos
		local dirx,diry,dirz = dir:get()
		local posx,posy,posz = startPos:get()
		obj = GO.castRay(posx,posy,posz,dirx,diry,dirz,50,1)
		if obj ~= nil then
			if obj:getTag() ~= "floor" then
				out = closestNode
				return out
			end
			table.insert(triedNodes,closestNode)
			tries = tries+1
		else
			table.insert(triedNodes,closestNode)
			tries = tries+1
		end
	end
end
