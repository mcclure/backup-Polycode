import sys
import CppHeaderParser
import os
import re

def createLUABindings(inputPath, prefix, mainInclude, libSmallName, libName, apiPath, apiClassPath, includePath, sourcePath, inheritInModuleFiles):	
	out = ""
	sout = ""
	
	lfout = ""
	
	sout += "#include \"%sLUA.h\"\n" % (prefix)
	sout += "#include \"%sLUAWrappers.h\"\n" % (prefix)
	sout += "#include \"PolyCoreServices.h\"\n\n"
	sout += "using namespace Polycode;\n\n"
	sout += "int luaopen_%s(lua_State *L) {\n" % (prefix)
	if prefix != "Polycode":
		sout += "CoreServices *inst = (CoreServices*)lua_topointer(L, 1);\n"
		sout += "CoreServices::setInstance(inst);\n"
	sout += "\tstatic const struct luaL_reg %sLib [] = {" % (libSmallName)
	
	out += "#pragma once\n\n"

	out += "extern \"C\" {\n\n"
	out += "#include <stdio.h>\n"
	out += "#include \"lua.h\"\n"
	out += "#include \"lualib.h\"\n"
	out += "#include \"lauxlib.h\"\n"
	out += "} // extern \"C\" \n\n"

	files = os.listdir(inputPath)
	filteredFiles = []
	for fileName in files:
		ignore = ["PolyGLSLProgram", "PolyGLSLShader", "PolyGLSLShaderModule", "PolyWinCore", "PolyCocoaCore", "PolyAGLCore", "PolySDLCore", "Poly_iPhone", "PolyGLES1Renderer", "PolyGLRenderer", "tinyxml", "tinystr", "OpenGLCubemap", "PolyiPhoneCore", "PolyGLES1Texture", "PolyGLTexture", "PolyGLVertexBuffer", "PolyThreaded", "PolyGLHeaders", "GLee"]
		if fileName.split(".")[1] == "h" and fileName.split(".")[0] not in ignore:
			filteredFiles.append(fileName)
			out += "#include \"%s\"\n" % (fileName)

	out += "\nnamespace Polycode {\n\n"
	
	if prefix == "Polycode":
		out += "class LuaEventHandler : public EventHandler {\n"
		out += "public:\n"
		out += "	LuaEventHandler() : EventHandler() {}\n"
		out += "	void handleEvent(Event *e) {\n"
		out += "		lua_rawgeti( L, LUA_REGISTRYINDEX, wrapperIndex );\n"
		out += "		lua_getfield(L, -1, \"__handleEvent\");\n"
		out += "		lua_rawgeti( L, LUA_REGISTRYINDEX, wrapperIndex );\n"
		out += "		lua_pushlightuserdata(L, e);\n"
		out += "		lua_call(L, 2, 0);\n"
		out += "	}\n"
		out += "	int wrapperIndex;\n"
		out += "	lua_State *L;\n"
		out += "};\n\n"
	
	for fileName in filteredFiles:
		inheritInModule = ["PhysicsSceneEntity", "CollisionScene", "CollisionSceneEntity"]
		
		# A file or comma-separated list of files can be given to specify classes which are "package owned"
		# and should not be inherited out of Polycode/. The files should contain one class name per line,
		# and the class name may be prefixed with a path (which will be ignored).
		if inheritInModuleFiles:
			for filename in inheritInModuleFiles.split(","):
				with open(filename) as f:
					for line in f.readlines():
						inheritInModule.append(line.strip().split("/",1)[-1]) # Strip whitespace, path/
					
		headerFile = "%s/%s" % (inputPath, fileName)
		print "Parsing %s" % fileName
		try:
			f = open(headerFile)
			contents = f.read().replace("_PolyExport", "")
			cppHeader = CppHeaderParser.CppHeader(contents, "string")
			ignore_classes = ["PolycodeShaderModule", "Object", "Threaded", "OpenGLCubemap", "ParticleEmitter"]

			for ckey in cppHeader.classes:
				print ">> Parsing class %s" % ckey
				c = cppHeader.classes[ckey]
	#			if ckey == "ParticleEmitter":
	#				print c
				lout = ""
				inherits = False
				if len(c["inherits"]) > 0:
					if c["inherits"][0]["class"] not in ignore_classes:
						if c["inherits"][0]["class"] in inheritInModule:
							lout += "require \"%s/%s\"\n\n" % (prefix, c["inherits"][0]["class"])
						else:
							lout += "require \"Polycode/%s\"\n\n" % (c["inherits"][0]["class"])
						lout += "class \"%s\" (%s)\n\n" % (ckey, c["inherits"][0]["class"])
						inherits = True
				if inherits == False:
					lout += "class \"%s\"\n\n" % ckey

				if len(c["methods"]["public"]) < 2 or ckey in ignore_classes:
					continue

				if ckey == "OSFileEntry":
					print c["methods"]["public"]
				parsed_methods = []
				ignore_methods = ["readByte32", "readByte16", "getCustomEntitiesByType", "Core", "Renderer", "Shader", "Texture", "handleEvent", "secondaryHandler", "getSTLString"]
				lout += "\n\n"

				pps = []
				for pp in c["properties"]["public"]:
					pp["type"] = pp["type"].replace("Polycode::", "")
					pp["type"] = pp["type"].replace("std::", "")
					if pp["type"].find("static ") != -1:
						if "defaltValue" in pp:
							lout += "%s = %s\n" % (pp["name"], pp["defaltValue"])
					else:
						#there are some bugs in the class parser that cause it to return junk
						if pp["type"].find("*") == -1 and pp["type"].find("vector") == -1 and pp["name"] != "setScale" and pp["name"] != "setPosition" and pp["name"] != "BUFFER_CACHE_PRECISION" and not pp["name"].isdigit():
							pps.append(pp)
						#if pp["type"] == "Number" or  pp["type"] == "String" or pp["type"] == "int" or pp["type"] == "bool":
						#	pps.append(pp)
						#else:
						#	print(">>> Skipping %s[%s %s]" % (ckey, pp["type"], pp["name"]))

				pidx = 0

				# hack to fix the lack of multiple inheritance
				#if ckey == "ScreenParticleEmitter" or ckey == "SceneParticleEmitter":
				#		pps.append({"name": "emitter", "type": "ParticleEmitter"})

				if len(pps) > 0:
					lout += "function %s:__index__(name)\n" % ckey
					for pp in pps:
						pp["type"] = pp["type"].replace("Polycode::", "")
						pp["type"] = pp["type"].replace("std::", "")
						if pidx == 0:
							lout += "\tif name == \"%s\" then\n" % (pp["name"])
						else:
							lout += "\telseif name == \"%s\" then\n" % (pp["name"])

						if pp["type"] == "Number" or  pp["type"] == "String" or pp["type"] == "int" or pp["type"] == "bool":
							lout += "\t\treturn %s.%s_get_%s(self.__ptr)\n" % (libName, ckey, pp["name"])
						elif (ckey == "ScreenParticleEmitter" or ckey == "SceneParticleEmitter") and pp["name"] == "emitter":
							lout += "\t\tlocal ret = %s(\"__skip_ptr__\")\n" % (pp["type"])
							lout += "\t\tret.__ptr = self.__ptr\n"
							lout += "\t\treturn ret\n"
						else:
							lout += "\t\tretVal = %s.%s_get_%s(self.__ptr)\n" % (libName, ckey, pp["name"])
							lout += "\t\tif Polycore.__ptr_lookup[retVal] ~= nil then\n"
							lout += "\t\t\treturn Polycore.__ptr_lookup[retVal]\n"
							lout += "\t\telse\n"
							lout += "\t\t\tPolycore.__ptr_lookup[retVal] = %s(\"__skip_ptr__\")\n" % (pp["type"])
							lout += "\t\t\tPolycore.__ptr_lookup[retVal].__ptr = retVal\n"
							lout += "\t\t\treturn Polycore.__ptr_lookup[retVal]\n"
							lout += "\t\tend\n"

						if not ((ckey == "ScreenParticleEmitter" or ckey == "SceneParticleEmitter") and pp["name"] == "emitter"):
							sout += "\t\t{\"%s_get_%s\", %s_%s_get_%s},\n" % (ckey, pp["name"], libName, ckey, pp["name"])
							out += "static int %s_%s_get_%s(lua_State *L) {\n" % (libName, ckey, pp["name"])
							out += "\tluaL_checktype(L, 1, LUA_TLIGHTUSERDATA);\n"
							out += "\t%s *inst = (%s*)lua_topointer(L, 1);\n" % (ckey, ckey)

							outfunc = "lua_pushlightuserdata"
							retFunc = ""
							if pp["type"] == "Number":
								outfunc = "lua_pushnumber"
							if pp["type"] == "String":
								outfunc = "lua_pushstring"
								retFunc = ".c_str()"
							if pp["type"] == "int":
								outfunc = "lua_pushinteger"
							if pp["type"] == "bool":
								outfunc = "lua_pushboolean"

							if pp["type"] == "Number" or  pp["type"] == "String" or pp["type"] == "int" or pp["type"] == "bool":
								out += "\t%s(L, inst->%s%s);\n" % (outfunc, pp["name"], retFunc)
							else:
								out += "\t%s(L, &inst->%s%s);\n" % (outfunc, pp["name"], retFunc)
							out += "\treturn 1;\n"
							out += "}\n\n"
						pidx = pidx + 1

					lout += "\tend\n"
					lout += "end\n"

				lout += "\n\n"
				pidx = 0
				if len(pps) > 0:
					lout += "function %s:__set_callback(name,value)\n" % ckey
					for pp in pps:
						pp["type"] = pp["type"].replace("Polycode::", "")
						pp["type"] = pp["type"].replace("std::", "")
						if pp["type"] == "Number" or  pp["type"] == "String" or pp["type"] == "int" or pp["type"] == "bool":
							if pidx == 0:
								lout += "\tif name == \"%s\" then\n" % (pp["name"])
							else:
								lout += "\telseif name == \"%s\" then\n" % (pp["name"])
							lout += "\t\t%s.%s_set_%s(self.__ptr, value)\n" % (libName, ckey, pp["name"])
							lout += "\t\treturn true\n"

							sout += "\t\t{\"%s_set_%s\", %s_%s_set_%s},\n" % (ckey, pp["name"], libName, ckey, pp["name"])
							out += "static int %s_%s_set_%s(lua_State *L) {\n" % (libName, ckey, pp["name"])
							out += "\tluaL_checktype(L, 1, LUA_TLIGHTUSERDATA);\n"
							out += "\t%s *inst = (%s*)lua_topointer(L, 1);\n" % (ckey, ckey)

							outfunc = "lua_topointer"
							if pp["type"] == "Number":
								outfunc = "lua_tonumber"
							if pp["type"] == "String":
								outfunc = "lua_tostring"
							if pp["type"] == "int":
								outfunc = "lua_tointeger"
							if pp["type"] == "bool":
								outfunc = "lua_toboolean"

							out += "\t%s param = %s(L, 2);\n" % (pp["type"], outfunc)
							out += "\tinst->%s = param;\n" % (pp["name"])

							out += "\treturn 0;\n"
							out += "}\n\n"
							pidx = pidx + 1
					if pidx != 0:
						lout += "\tend\n"
					lout += "\treturn false\n"
					lout += "end\n"


				lout += "\n\n"
				for pm in c["methods"]["public"]:
					if pm["name"] in parsed_methods or pm["name"].find("operator") > -1 or pm["name"] in ignore_methods:
						continue

					if pm["name"] == "~"+ckey or pm["rtnType"].find("<") > -1:
						out += ""
					else:
						basicType = False
						voidRet = False
						if pm["name"] == ckey:
							sout += "\t\t{\"%s\", %s_%s},\n" % (ckey, libName, ckey)
							out += "static int %s_%s(lua_State *L) {\n" % (libName, ckey)
							idx = 1
						else:
							sout += "\t\t{\"%s_%s\", %s_%s_%s},\n" % (ckey, pm["name"], libName, ckey, pm["name"])
							out += "static int %s_%s_%s(lua_State *L) {\n" % (libName, ckey, pm["name"])

							if pm["rtnType"].find("static ") == -1:
								out += "\tluaL_checktype(L, 1, LUA_TLIGHTUSERDATA);\n"
								out += "\t%s *inst = (%s*)lua_topointer(L, 1);\n" % (ckey, ckey)
							idx = 2
						paramlist = []
						lparamlist = []
						for param in pm["parameters"]:
							if not param.has_key("type"):
								continue
							if param["type"] == "0":
								continue
							param["type"] = param["type"].replace("Polycode::", "")
							param["type"] = param["type"].replace("std::", "")
							param["type"] = param["type"].replace("const", "")
							param["type"] = param["type"].replace("&", "")
							param["type"] = param["type"].replace(" ", "")
							param["type"] = param["type"].replace("long", "long ")
							param["type"] = param["type"].replace("unsigned", "unsigned ")

							param["name"] = param["name"].replace("end", "_end").replace("repeat", "_repeat")
							if"type" in param:
								luatype = "LUA_TLIGHTUSERDATA"
								checkfunc = "lua_islightuserdata"
								if param["type"].find("*") > -1:
									luafunc = "(%s)lua_topointer" % (param["type"].replace("Polygon", "Polycode::Polygon").replace("Rectangle", "Polycode::Rectangle"))
								elif param["type"].find("&") > -1:
									luafunc = "*(%s*)lua_topointer" % (param["type"].replace("const", "").replace("&", "").replace("Polygon", "Polycode::Polygon").replace("Rectangle", "Polycode::Rectangle"))
								else:
									luafunc = "*(%s*)lua_topointer" % (param["type"].replace("Polygon", "Polycode::Polygon").replace("Rectangle", "Polycode::Rectangle"))
								lend = ".__ptr"
								if param["type"] == "int" or param["type"] == "unsigned int":
									luafunc = "lua_tointeger"
									luatype = "LUA_TNUMBER"
									checkfunc = "lua_isnumber"
									lend = ""
								if param["type"] == "bool":
									luafunc = "lua_toboolean"
									luatype = "LUA_TBOOLEAN"
									checkfunc = "lua_isboolean"
									lend = ""
								if param["type"] == "Number" or param["type"] == "float" or param["type"] == "double":
									luatype = "LUA_TNUMBER"
									luafunc = "lua_tonumber"
									checkfunc = "lua_isnumber"
									lend = ""
								if param["type"] == "String":
									luatype = "LUA_TSTRING"
									luafunc = "lua_tostring"
									checkfunc = "lua_isstring"
									lend = ""

								param["type"] = param["type"].replace("Polygon", "Polycode::Polygon").replace("Rectangle", "Polycode::Rectangle")

								if "defaltValue" in param:
									if checkfunc != "lua_islightuserdata" or (checkfunc == "lua_islightuserdata" and param["defaltValue"] == "NULL"):
										#param["defaltValue"] = param["defaltValue"].replace(" 0f", ".0f")
										param["defaltValue"] = param["defaltValue"].replace(": :", "::")
										#param["defaltValue"] = param["defaltValue"].replace("0 ", "0.")
										param["defaltValue"] = re.sub(r'([0-9]+) ([0-9])+', r'\1.\2', param["defaltValue"])

										out += "\t%s %s;\n" % (param["type"], param["name"])
										out += "\tif(%s(L, %d)) {\n" % (checkfunc, idx)
										out += "\t\t%s = %s(L, %d);\n" % (param["name"], luafunc, idx)
										out += "\t} else {\n"
										out += "\t\t%s = %s;\n" % (param["name"], param["defaltValue"])
										out += "\t}\n"
									else:
										out += "\tluaL_checktype(L, %d, %s);\n" % (idx, luatype);
										if param["type"] == "String":
											out += "\t%s %s = String(%s(L, %d));\n" % (param["type"], param["name"], luafunc, idx)
										else:
											out += "\t%s %s = %s(L, %d);\n" % (param["type"], param["name"], luafunc, idx)
								else:
									out += "\tluaL_checktype(L, %d, %s);\n" % (idx, luatype);
									if param["type"] == "String":
										out += "\t%s %s = String(%s(L, %d));\n" % (param["type"], param["name"], luafunc, idx)
									else:
										out += "\t%s %s = %s(L, %d);\n" % (param["type"], param["name"], luafunc, idx)
								paramlist.append(param["name"])

								lparamlist.append(param["name"]+lend)
								idx = idx +1

						if pm["name"] == ckey:
							if ckey == "EventHandler":
								out += "\tLuaEventHandler *inst = new LuaEventHandler();\n"
								out += "\tinst->wrapperIndex = luaL_ref(L, LUA_REGISTRYINDEX );\n"
								out += "\tinst->L = L;\n"
							else:
								out += "\t%s *inst = new %s(%s);\n" % (ckey, ckey, ", ".join(paramlist))
							out += "\tlua_pushlightuserdata(L, (void*)inst);\n"
							out += "\treturn 1;\n"
						else:
							if pm["rtnType"].find("static ") == -1:
								call = "inst->%s(%s)" % (pm["name"], ", ".join(paramlist))
							else:
								call = "%s::%s(%s)" % (ckey, pm["name"], ", ".join(paramlist))
							if pm["rtnType"] == "void" or pm["rtnType"] == "static void" or pm["rtnType"] == "virtual void" or pm["rtnType"] == "inline void":
								out += "\t%s;\n" % (call)
								basicType = True
								voidRet = True
								out += "\treturn 0;\n"
							else:
								outfunc = "lua_pushlightuserdata"
								retFunc = ""
								basicType = False
								if pm["rtnType"] == "Number" or  pm["rtnType"] == "inline Number":
									outfunc = "lua_pushnumber"
									basicType = True
								if pm["rtnType"] == "String" or pm["rtnType"] == "static String":
									outfunc = "lua_pushstring"
									basicType = True
									retFunc = ".c_str()"
								if pm["rtnType"] == "int" or pm["rtnType"] == "static int" or  pm["rtnType"] == "size_t" or pm["rtnType"] == "static size_t" or pm["rtnType"] == "long" or pm["rtnType"] == "unsigned int" or pm["rtnType"] == "static long":
									outfunc = "lua_pushinteger"
									basicType = True
								if pm["rtnType"] == "bool" or pm["rtnType"] == "static bool" or pm["rtnType"] == "virtual bool":
									outfunc = "lua_pushboolean"
									basicType = True

								if pm["rtnType"].find("*") > -1:
									out += "\tvoid *ptrRetVal = (void*)%s%s;\n" % (call, retFunc)
									out += "\tif(ptrRetVal == NULL) {\n"
									out += "\t\tlua_pushnil(L);\n"
									out += "\t} else {\n"
									out += "\t\t%s(L, ptrRetVal);\n" % (outfunc)
									out += "\t}\n"
								elif basicType == True:
									out += "\t%s(L, %s%s);\n" % (outfunc, call, retFunc)
								else:
									className = pm["rtnType"].replace("const", "").replace("&", "").replace("inline", "").replace("virtual", "").replace("static", "")
									if className == "Polygon":
										className = "Polycode::Polygon"
									if className == "Rectangle":
										className = "Polycode::Rectangle"
									out += "\t%s *retInst = new %s();\n" % (className, className)
									out += "\t*retInst = %s;\n" % (call)
									out += "\t%s(L, retInst);\n" % (outfunc)
								out += "\treturn 1;\n"
						out += "}\n\n"

						if pm["name"] == ckey:
							lout += "function %s:%s(...)\n" % (ckey, ckey)
							if inherits:
								lout += "\tif type(arg[1]) == \"table\" and count(arg) == 1 then\n"
								lout += "\t\tif \"\"..arg[1]:class() == \"%s\" then\n" % (c["inherits"][0]["class"])
								lout += "\t\t\tself.__ptr = arg[1].__ptr\n"
								lout += "\t\t\treturn\n"
								lout += "\t\tend\n"
								lout += "\tend\n"
							lout += "\tfor k,v in pairs(arg) do\n"
							lout += "\t\tif type(v) == \"table\" then\n"
							lout += "\t\t\tif v.__ptr ~= nil then\n"
							lout += "\t\t\t\targ[k] = v.__ptr\n"
							lout += "\t\t\tend\n"
							lout += "\t\tend\n"
							lout += "\tend\n"
							lout += "\tif self.__ptr == nil and arg[1] ~= \"__skip_ptr__\" then\n"
							if ckey == "EventHandler":
								lout += "\t\tself.__ptr = %s.%s(self)\n" % (libName, ckey)
							else:
								lout += "\t\tself.__ptr = %s.%s(unpack(arg))\n" % (libName, ckey)
							lout += "\t\tPolycore.__ptr_lookup[self.__ptr] = self\n"
							lout += "\tend\n"
							lout += "end\n\n"
						else:
							lout += "function %s:%s(%s)\n" % (ckey, pm["name"], ", ".join(paramlist))
							if pm["rtnType"].find("static ") == -1:
								if len(lparamlist):
									lout += "\tlocal retVal = %s.%s_%s(self.__ptr, %s)\n" % (libName, ckey, pm["name"], ", ".join(lparamlist))
								else:
									lout += "\tlocal retVal =  %s.%s_%s(self.__ptr)\n" % (libName, ckey, pm["name"])
							else:
								if len(lparamlist):
									lout += "\tlocal retVal = %s.%s_%s(%s)\n" % (libName, ckey, pm["name"], ", ".join(lparamlist))
								else:
									lout += "\tlocal retVal =  %s.%s_%s()\n" % (libName, ckey, pm["name"])

							if not voidRet:
								if basicType == True:
									lout += "\treturn retVal\n"
								else:
									className = pm["rtnType"].replace("const", "").replace("&", "").replace("inline", "").replace("virtual", "").replace("static", "").replace("*","").replace(" ", "")
									lout += "\tif retVal == nil then return nil end\n"
									lout += "\tif Polycore.__ptr_lookup[retVal] ~= nil then\n"
									lout += "\t\treturn Polycore.__ptr_lookup[retVal]\n"
									lout += "\telse\n"
									lout += "\t\tPolycore.__ptr_lookup[retVal] = %s(\"__skip_ptr__\")\n" % (className)
									lout += "\t\tPolycore.__ptr_lookup[retVal].__ptr = retVal\n"
									lout += "\t\treturn Polycore.__ptr_lookup[retVal]\n"
									lout += "\tend\n"
							lout += "end\n\n"

					parsed_methods.append(pm["name"])

				#cleanup
				sout += "\t\t{\"delete_%s\", %s_delete_%s},\n" % (ckey, libName, ckey)
				out += "static int %s_delete_%s(lua_State *L) {\n" % (libName, ckey)
				out += "\tluaL_checktype(L, 1, LUA_TLIGHTUSERDATA);\n"
				out += "\t%s *inst = (%s*)lua_topointer(L, 1);\n" % (ckey, ckey)
				out += "\tdelete inst;\n"
				out += "\treturn 0;\n"
				out += "}\n\n"

				lout += "\n\n"
				lout += "function %s:__delete()\n" % (ckey)
				lout += "\tPolycore.__ptr_lookup[self.__ptr] = nil\n"
				lout += "\t%s.delete_%s(self.__ptr)\n" % (libName, ckey)
				lout += "end\n"
				if ckey == "EventHandler":
					lout += "\n\n"
					lout += "function EventHandler:__handleEvent(event)\n"
					lout += "\tevt = Event(\"__skip_ptr__\")\n"
					lout += "\tevt.__ptr = event\n"
					lout += "\tself:handleEvent(evt)\n"
					#lout += "\tself:handleEvent(event)\n"
					lout += "end\n"
				lfout += "require \"%s/%s\"\n" % (prefix, ckey)
				fout = open("%s/%s.lua" % (apiClassPath, ckey), "w")
				fout.write(lout)
		except CppHeaderParser.CppParseError,  e:
			print e
			sys.exit(1)

	out += "} // namespace Polycode\n"
	
	sout += "\t\t{NULL, NULL}\n"
	sout += "\t};\n"
	sout += "\tluaL_openlib(L, \"%s\", %sLib, 0);\n" % (libName, libSmallName)
	sout += "\treturn 1;\n"
	sout += "}"
	
	
	shout = ""
	shout += "#pragma once\n"
	shout += "#include <%s>\n" % (mainInclude)
	shout += "extern \"C\" {\n"
	shout += "#include <stdio.h>\n"
	shout += "#include \"lua.h\"\n"
	shout += "#include \"lualib.h\"\n"
	shout += "#include \"lauxlib.h\"\n"
	shout += "int _PolyExport luaopen_%s(lua_State *L);\n" % (prefix)
	shout += "}\n"
	
	fout = open("%s/%sLUA.h" % (includePath, prefix), "w")
	fout.write(shout)

	fout = open("%s/%s.lua" % (apiPath, prefix), "w")
	fout.write(lfout)
	
	fout = open("%s/%sLUAWrappers.h" % (includePath, prefix), "w")
	fout.write(out)
	
	fout = open("%s/%sLUA.cpp" % (sourcePath, prefix), "w")
	fout.write(sout)
	
	#print cppHeader

returncode = 0

if len(sys.argv) < 10:
	print ("Usage:\n%s [input path] [prefix] [main include] [lib small name] [lib name] [api path] [api class-path] [include path] [source path] [inherit-in-module-file path (optional)]" % (sys.argv[0]))
	sys.exit(1)
else:
	createLUABindings(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8], sys.argv[9], sys.argv[10] if len(sys.argv)>10 else None)