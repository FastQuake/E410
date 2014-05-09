require "networkutils"
require "vector"
require "ship"

peers = {}
peerID = 0
playerSpeed = 15
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
	elseif data[1] == "stop" then
		if data[2] == "forward" then
			p.fwd = 0
		elseif data[2] == "backward" then
			p.fwd = -0
		elseif data[2] == "right" then
			p.right = 0
		elseif data[2] == "left" then
			p.right = -0
		end
	elseif data[1] == "turn" then
		if p.flying == true then
			ship.rot = Vector.create(0,data[2],0) + ship.rot
			ship.deltaRot = Vector.create(0,data[2]/2,0)
		else
			local rot = Vector.create(p.model:getRot()) + Vector.create(0,-data[2],0) 
			p.model:setRot(rot:get())
		end
	elseif data[1] == "cast" then
		local obj = GO.castRay(data[2],data[3],data[4],data[5],data[6],data[7],15)
		if obj ~= nil then
			print("Looking at: "..obj:getTag())
			if obj:getTag() == "chair" then
				p.flying = true
				network.sendPacket(id, "fly")
			end
		end
	end
end

function onPeerDisconnect(id)
	print("peer "..id.." disconnected")
	local p = Peer.getPeer(peers, id)
	p.model:remove()
end

spaceObjects = {}
function init()
	ship = PlayerShip.create()

	shit = GO.loadIQM("monkey.iqm","monkey")
	shit:setBoxBody()
	shit:setMass(1.0)
	shit:setPos(3,5,0)

	e = GO.loadIQM("ship_ext.iqm","eship")
	e:setBoxBody()
	e:setMass(10)
	e:setG(0,0,0)
	e:setPos(150,0,0)
	e:setRot(180,0,0)
	e:setActivation(true)

	e2 = GO.loadIQM("ship_ext.iqm","eship2")
	e2:setBoxBody()
	e2:setMass(10)
	e2:setG(0,0,0)
	e2:setPos(-150,0,0)
	--e2:setRot(180,0,0)
	e2:setActivation(true)


	table.insert(spaceObjects,e)
	table.insert(spaceObjects,e2)
end

delta = 0
function update(dt)
	delta = dt
	for k,v in pairs(peers) do
		local fwd = Vector.create(v.model:getLookat())
		local right = Vector.cross(Vector.create(0,1,0),fwd)
		local out = Vector.scalarMul(v.fwd,fwd) + Vector.scalarMul(v.right,right)
		local vel = Vector.create(v.model:getVelocity())
		if out ~= Vector.create(0,0,0) then
			out = out:normalize()
			out = Vector.scalarMul(playerSpeed, out)
			out.y = vel.y
			if v.flying == true then
				local fwd = Vector.create(ship.ship:getLookat())
				local right = Vector.cross(Vector.create(0,1,0),fwd)
				local out = Vector.scalarMul(v.fwd,fwd) + Vector.scalarMul(v.right,right)
				out = Vector.scalarMul(v.fwd,fwd) + Vector.scalarMul(v.right,right)
				ship.vel = Vector.scalarMul(50,out:normalize())
			else
				v.model:setVelocity(out:get())
			end
		else
			if v.flying == true then
				ship.vel = Vector.create(0,0,0)
			else
				v.model:setVelocity(0,vel.y,0)
			end
		end
	end

	ship:relativeMove(spaceObjects)
end
