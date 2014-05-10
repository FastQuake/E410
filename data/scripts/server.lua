require "networkutils"
require "vector"
require "ai"

peers = {}
peerID = 0
playerSpeed = 5
function onPeerConnect(id)
	print("Got connection from peer "..id)
	network.sendPacket(id, "player".. peerID)
	local p = Peer.create("guy.iqm", "player"..peerID)
	table.insert(peers,p)
end

function onReceivePacket(id, data)
	local out = ""
	for k,v in pairs(data) do
		out = out.." "..v
	end
	--print(out)
	local p = Peer.getPeer(peers, id)
	if data[1] == "move" then
		if data[2] == "forward" then
			p.fwd = 1
		elseif data[2] == "backward" then
			p.fwd = -1
		elseif data[2] == "right" then
			p.right = 1
		elseif data[2] == "left" then
			p.right = -1
		end
		network.sendPacket(-1, "animate "..p.model:getID())
	elseif data[1] == "stop" then
		if data[2] == "forward" then
			p.fwd = 0
		elseif data[2] == "backward" then
			p.fwd = 0
		elseif data[2] == "right" then
			p.right = 0
		elseif data[2] == "left" then
			p.right = 0
		end
		if p.right == 0 and p.fwd == 0 then
			network.sendPacket(-1, "stopanimate "..p.model:getID())
		end
	elseif data[1] == "turn" then
		local rot = Vector.create(p.model:getRot()) + Vector.create(0,-data[2],0) 
		p.model:setRot(rot:get())
	elseif data[1] == "cast" then
		local obj = GO.castRay(data[2],data[3],data[4],data[5],data[6],data[7],15,4)
		if obj ~= nil then
			print("Looking at: "..obj:getTag())
			if obj:getTag() == "chair" then
				p.flying = true
				network.sendPacket(id, "fly")
			end
		end
	elseif data[1] == "jump" then
		local x,y,z = p.model:getPos()
		local obj = GO.castRay(x,y,z,0,-1,0,0.4,4)
		if obj ~= nil then
			x,y,z = p.model:getVelocity()
			p.model:setVelocity(x,playerSpeed,z)
		end
	end
end

function onPeerDisconnect(id)
	print("peer "..id.." disconnected")
	local p = Peer.getPeer(peers, id)
	p.model:remove()
end

function init()
	shit = GO.loadIQM("monkey.iqm","monkey")
	--shit:setTriangleBody()
	shit:setBoxBody()
	shit:setMass(1.0)
	shit:setPos(3,5,0)

	floor = GO.loadIQM("level1.iqm", "floor")
	floor:setTriangleBody()
	floor:setMass(0)
	--floor:setScale(100,1,100)
	--floor:setPos(0,-1,0)
	AIManager.addMonster()
	AIManager.addNode(1,-0.14,-3.5,3.0);AIManager.addNeighbors(1,{2,3}) --left ramp bottom
	AIManager.addNode(2,-0.14,-3.5,-10);AIManager.addNeighbors(2,{1,4}) --right ramp bottom
	AIManager.addNode(3,8.6,-2.2,3.0);AIManager.addNeighbors(3,{4,5,1}) --left ramp top
	AIManager.addNode(4,8.6,-2.2,-10);AIManager.addNeighbors(4,{3,6,2}) --right ramp top
	AIManager.addNode(5,15.5,-2.2,3.0);AIManager.addNeighbors(5,{3,6}) --Left door inside
	AIManager.addNode(6,15.5,-2.2,-10);AIManager.addNeighbors(6,{5,7,4}) --Right door inside
	AIManager.addNode(7,20,-2.2,-4);AIManager.addNeighbors(7,{8,6,5,7}) --big ramp bottom
	AIManager.addNode(8,28.6,1.14,-4);AIManager.addNeighbors(8,{7}) --big ramp top
end

delta = 0
function update(dt)
	--print("running")
	delta = dt
	for k,v in pairs(peers) do
		local fwd = Vector.create(v.model:getLookat())
		local right = Vector.cross(fwd,Vector.create(0,1,0))
		local out = Vector.scalarMul(v.fwd,fwd) + Vector.scalarMul(v.right,right)
		local vel = Vector.create(v.model:getVelocity())
		if out ~= Vector.create(0,0,0) then
			out = out:normalize()
			out = Vector.scalarMul(playerSpeed, out)
			out.y = vel.y
			v.model:setVelocity(out:get())
		else
			v.model:setVelocity(0,vel.y,0)
		end
	end
end
