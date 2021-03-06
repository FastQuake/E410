require "networkutils"

local nodes = {}
require "networkutils"
local monsters = {}
AIManager = {}
bulletd = 300
wave = 1
function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

function tableContains(t,searchv)
	for k,v in pairs(t) do
		if v == searchv then
			return true
		end
	end
	return false
end

function getAiFromObj(obj)
	for k,v in pairs(monsters) do
		if v.model:getID() == obj:getID() then
			return v
		end
	end
	return nil
end

function removeMonster(obj)
	for k,v in pairs(monsters) do
		if v.model:getID() == obj:getID() then
			v.model:remove()
			__serverDelete(v.model:getID())
			table.remove(monsters,k)
		end
	end
end

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
	nodes[id] = node
end

function AIManager.addNeighbors(id1,ids)
	for k,v in pairs(ids) do
		local node = nodes[id1]
		table.insert(node.neighbors,nodes[v])
	end
end

function AIManager.getNode(id)
	return nodes[id]
end

function AIManager.getMonsters()
	return monsters
end

function AIManager.addMonster(numPlayers, pos)
	local monster = {}
	monster.model = GO.loadIQM("robit.iqm","monster")
	--monster.model:setBoxBody()
	monster.model:setExtBoxBody(-0.5,0,-0.5,0.5,2.5,0.5)
	monster.model:setActivation(true)
	monster.model:lockAxis(0,0,0)
	monster.model:setPos(pos:get())
	monster.pos = pos
	monster.path = {}
	monster.targetPlayer = math.random(0,numPlayers-1)
	monster.bulletTimer = 0
	monster.maxtime = (math.random()*0.5)+0.3
	monster.hp = 100
	table.insert(monsters,monster)
end

function AIManager.findVisibleNode(startPos)
	startPos.y = startPos.y + 2
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
		if dir == Vector.create(0,0,0) then
			out = closestNode
			return out
		else
			local dirx,diry,dirz = dir:get()
			local posx,posy,posz = startPos:get()
			local obj,x,y,z = GO.castRay(posx,posy,posz,dirx,diry,dirz,50,1)
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
	return 1
end

function AIManager.buildMonsterPaths()
	for k,v in pairs(monsters) do
		local peer = Peer.getPeer(peers,v.targetPlayer)
		local pvec = Vector.create(peer.model:getPos())
		v.path = {}
		local ignoreList = {}
		local nodehere = AIManager.findVisibleNode(v.pos)
		local nodethere = AIManager.findVisibleNode(pvec)
		--print(nodethere.model:getTag())
		table.insert(v.path,nodehere)
		table.insert(ignoreList,nodehere)
		local pathComplete = false
		while pathComplete == false do
			local closestNode = {}
			local distance = 0
			local min = 9999
			local dead = 0
			local endNode = v.path[#v.path]
			local neighbors = endNode.neighbors
			for l,w in pairs(neighbors) do
				local ignored = false
				if tableContains(ignoreList,w) == true then
					ignored = true
				end
				if ignored == true then
					dead = dead+1
					if dead == tablelength(neighbors) then
						table.insert(ignoreList,v.path[#v.path])
						table.remove(v.path)
						break
					end
				else
					distance = Vector.distance(w.pos,nodethere.pos)
					if distance < min then
						min = distance
						closestNode = w
					end
				end
			end
			if tableContains(ignoreList,closestNode) == false then
				table.insert(v.path,closestNode)
				table.insert(ignoreList,closestNode)
			end
			if v.path[#v.path] == nodethere then
				--print("PATH DONE")
				pathComplete = true
			end
		end
		v.targetNodeNum = 1
	end
end

function AIManager.spawnWave()
	local numMonsters = math.ceil(0.5*wave)
	if numMonsters > 10 then
		numMonsters = 10
	end
	for i=1,numMonsters do
		--local p = Vector.create(math.random(-4,4),0,math.random(-4,4))
		local p = nodes[math.random(1,#nodes)].pos
		p.x = p.x + math.random()-0.5
		p.y = p.y + 1
		p.z = p.z + math.random()-0.5
		AIManager.addMonster(#peers,p)
	end
	AIManager.buildMonsterPaths()
end

function AIManager.stepMonsters(peers, dt)
	if #monsters == 0 then
		wave = wave+1
		network.sendPacket(-1, "wave")
		AIManager.spawnWave()
	end
	for k,v in pairs(monsters) do
		local targetPos = Vector.create(Peer.getPeer(peers,v.targetPlayer).model:getPos())
		targetPos.y = targetPos.y+1
		local dir = (targetPos-Vector.create(v.model:getPos())):normalize()
		local pp = Vector.create(v.model:getPos())
		if math.abs(pp.x) > 50 or math.abs(pp.y) > 50 or math.abs(pp.z) > 50 then
				network.sendPacket(-1, "explode "..pp.x.." "..pp.y.." "..pp.z)
				removeMonster(v.model)
				break
		end
		pp.y = pp.y+1
		pp = pp+dir
		local obj = GO.castRay(pp.x,pp.y,pp.z,dir.x,dir.y,dir.z,100,4)
		if obj ~= nil then
			if obj:getTag():sub(1,6) == "player" then
				local oldv = Vector.create(v.model:getVelocity())
				local vel = dir:copy()
				vel.x = 3*vel.x
				vel.z = 3*vel.z
				vel.y = oldv.y
				v.model:setVelocity(Vector.scalarMul(1,vel):get())
				if vel == Vector.create(0,0,0) then
					network.sendPacket(-1, "stopanimate "..v.model:getID())	
				else
					network.sendPacket(-1, "animate "..v.model:getID())
				end
			end
		else
			if v.targetNodeNum < #v.path then
				local oldv = Vector.create(v.model:getVelocity())
				v.pos = Vector.create(v.model:getPos())
				local vel = (v.path[v.targetNodeNum].pos - v.pos):normalize()
				vel.x = 3*vel.x
				vel.z = 3*vel.z
				vel.y = oldv.y
				if vel == Vector.create(0,0,0) then
					network.sendPacket(-1, "stopanimate "..v.model:getID())	
				else
					network.sendPacket(-1, "animate "..v.model:getID())
				end
				v.model:setVelocity(Vector.scalarMul(1,vel):get())
				if Vector.distance(v.path[v.targetNodeNum].pos,v.pos) < 1.0 then
					v.targetNodeNum = v.targetNodeNum+1
				end
			end
		end
		local p2 = Vector.create(v.model:getPos())
		p2.y = p2.y+1.5
		local dir = Vector.normalize(targetPos-p2)

		local dirx,diry,dirz = dir:get()
		local yaw = math.acos(Vector.dot(Vector.create(1,0,0),Vector.create(dirx,0,dirz)))
		yaw = math.deg(yaw)
		if dirz > 0 then
			yaw = -yaw
		end
		if v.bulletTimer > v.maxtime then
			local rx = (math.random()/3)-(1/6)
			local ry = (math.random()/3)-(1/6)
			local rz = (math.random()/3)-(1/6)
			local obj,x,y,z = GO.castRay(p2.x,p2.y,p2.z,dirx+rx,diry+ry,dirz+rz,100,4)
			v.model:setRot(0,yaw,0)
			if obj ~= nil then
				network.sendPacket(-1,"shoot "..p2.x.." "..p2.y.." "..p2.z.." "..x.." "..y.." "..z)
				if obj:getTag():sub(1,6) == "player" then
					local id = Peer.getIDFromObject(peers,obj) 
					network.sendPacket(id,"hit")
				end
			else
				network.sendPacket(-1,"shoot "..p2.x.." "..p2.y.." "..p2.z.." "..bulletd*(dirx+rx).." "..bulletd*(diry+ry).." "..bulletd*(dirz+rz))
			end
			v.bulletTimer = 0
		end
		v.bulletTimer = v.bulletTimer + dt
	end
end
