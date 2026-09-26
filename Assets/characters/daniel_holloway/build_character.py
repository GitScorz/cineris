"""Build a compact, PS2-inspired Daniel Holloway character in Blender 4.5.

Run from the repository's graphics-research directory:
  D:\\Blender\\blender.exe -b --python scenes/characters/daniel_holloway/build_character.py
"""

import bpy
import math
import random
from mathutils import Vector
from pathlib import Path


ROOT = Path(__file__).resolve().parent
random.seed(51)
bpy.ops.object.select_all(action="SELECT")
bpy.ops.object.delete(use_global=False)


def texture(name, color, variation=0.045):
    image = bpy.data.images.new(name, width=64, height=64, alpha=False)
    pixels = []
    for y in range(64):
        for x in range(64):
            grain = random.uniform(-variation, variation)
            broad = 0.025 * math.sin(x * 0.27 + y * 0.13)
            for channel in color:
                pixels.append(max(0.0, min(1.0, channel + grain + broad)))
            pixels.append(1.0)
    image.pixels = pixels
    image.filepath_raw = str(ROOT / f"{name}.png")
    image.file_format = "PNG"
    image.save()

    material = bpy.data.materials.new(name)
    material.diffuse_color = (*color, 1.0)
    material.use_nodes = True
    nodes = material.node_tree.nodes
    bsdf = nodes.get("Principled BSDF")
    bsdf.inputs["Roughness"].default_value = 0.95
    tex = nodes.new("ShaderNodeTexImage")
    tex.image = image
    material.node_tree.links.new(tex.outputs["Color"], bsdf.inputs["Base Color"])
    return material


jacket = texture("worn_brown_leather", (0.21, 0.135, 0.085), 0.075)
jacket_light = texture("leather_edges", (0.28, 0.19, 0.12), 0.055)
jacket_dark = texture("leather_seams", (0.10, 0.068, 0.045), 0.025)
shirt = texture("faded_teal_shirt", (0.105, 0.17, 0.16), 0.045)
pants = texture("charcoal_trousers", (0.11, 0.105, 0.095), 0.045)
boots = texture("dark_boots", (0.09, 0.065, 0.048), 0.04)
skin = texture("weathered_skin", (0.48, 0.34, 0.25), 0.05)
shadow_skin = texture("face_shadows", (0.27, 0.18, 0.13), 0.04)
hair = texture("dark_brown_hair", (0.105, 0.075, 0.055), 0.04)
eye = texture("dark_eyes", (0.075, 0.085, 0.075), 0.01)
metal = texture("dull_metal", (0.37, 0.34, 0.28), 0.025)


def finish(obj, name, material):
    obj.name = name
    obj.data.materials.append(material)
    for polygon in obj.data.polygons:
        polygon.use_smooth = False
    return obj


def box(name, location, scale, material, bevel=0):
    bpy.ops.mesh.primitive_cube_add(size=1, location=location)
    obj = bpy.context.object
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    if bevel:
        mod = obj.modifiers.new("soft block edges", "BEVEL")
        mod.width = bevel
        mod.segments = 1
        bpy.context.view_layer.objects.active = obj
        bpy.ops.object.modifier_apply(modifier=mod.name)
    return finish(obj, name, material)


def ellipsoid(name, location, scale, material, segments=10, rings=6):
    bpy.ops.mesh.primitive_uv_sphere_add(segments=segments, ring_count=rings, location=location)
    obj = bpy.context.object
    obj.scale = scale
    return finish(obj, name, material)


def limb(name, start, end, r0, r1, material, sides=8):
    start, end = Vector(start), Vector(end)
    direction = end - start
    bpy.ops.mesh.primitive_cone_add(
        vertices=sides, radius1=r0, radius2=r1, depth=direction.length,
        location=(start + end) / 2,
    )
    obj = bpy.context.object
    obj.rotation_euler = direction.to_track_quat("Z", "Y").to_euler()
    return finish(obj, name, material)


def plane(name, vertices, material):
    mesh = bpy.data.meshes.new(name)
    mesh.from_pydata(vertices, [], [tuple(range(len(vertices)))])
    mesh.update()
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.collection.objects.link(obj)
    return finish(obj, name, material)


# Trousers, slight asymmetry, and chunky shoes.
box("hips", (0, 0, 0.94), (0.37, 0.23, 0.24), pants, 0.055)
for sign, word in [(-1, "L"), (1, "R")]:
    x = sign * 0.115
    lean = 0.025 if sign == 1 else -0.015
    limb(f"{word} upper trouser", (x, 0, 0.93), (x + lean, 0, 0.52), 0.115, 0.10, pants)
    limb(f"{word} lower trouser", (x + lean, 0, 0.52), (x + lean, -0.015, 0.14), 0.105, 0.085, pants)
    box(f"{word} ankle fold", (x + lean, -0.01, 0.17), (0.21, 0.22, 0.085), pants, 0.012)
    box(f"{word} boot", (x + lean, -0.055, 0.085), (0.22, 0.36, 0.16), boots, 0.035)
    box(f"{word} boot sole", (x + lean, -0.065, 0.02), (0.23, 0.39, 0.04), jacket_dark, 0.012)
    for z in (0.11, 0.13):
        box(f"{word} boot lace {z}", (x + lean, -0.236, z), (0.10, 0.006, 0.006), jacket_light)

# Shirt visible at the open jacket and raised collar.
box("shirt torso", (0, -0.005, 1.25), (0.37, 0.23, 0.53), shirt, 0.04)
limb("neck", (0, 0, 1.49), (0, 0, 1.59), 0.075, 0.07, skin)
plane("shirt opening shadow", [(-0.095, -0.125, 1.49), (0.095, -0.125, 1.49), (0, -0.127, 1.35)], jacket_dark)
for sign, word in [(-1, "L"), (1, "R")]:
    box(f"{word} shirt collar", (sign * 0.085, -0.14, 1.51), (0.15, 0.035, 0.11), shirt, 0.008)

# Jacket body is split to reveal the shirt. Dark side panels suggest worn seams.
box("jacket back", (0, 0.105, 1.25), (0.49, 0.13, 0.57), jacket, 0.04)
for sign, word in [(-1, "L"), (1, "R")]:
    x = sign * 0.165
    box(f"{word} jacket front", (x, -0.12, 1.245), (0.20, 0.12, 0.55), jacket, 0.025)
    box(f"{word} lapel", (sign * 0.116, -0.195, 1.425), (0.10, 0.035, 0.22), jacket_light, 0.006)
    box(f"{word} upper pocket", (sign * 0.16, -0.189, 1.36), (0.14, 0.018, 0.10), jacket_dark, 0.004)
    box(f"{word} pocket flap", (sign * 0.16, -0.206, 1.405), (0.16, 0.02, 0.035), jacket_light, 0.003)
    box(f"{word} lower pocket", (sign * 0.16, -0.19, 1.10), (0.17, 0.02, 0.11), jacket_dark, 0.004)
    box(f"{word} lower pocket flap", (sign * 0.16, -0.205, 1.15), (0.18, 0.025, 0.036), jacket_light, 0.003)
    box(f"{word} side seam", (sign * 0.247, 0, 1.25), (0.012, 0.22, 0.48), jacket_dark)
    limb(f"{word} shoulder", (sign * 0.22, 0, 1.49), (sign * 0.32, 0, 1.43), 0.12, 0.115, jacket)
    limb(f"{word} upper sleeve", (sign * 0.32, 0, 1.43), (sign * 0.34, -0.015, 1.18), 0.116, 0.097, jacket)
    limb(f"{word} forearm sleeve", (sign * 0.34, -0.015, 1.18), (sign * 0.35, -0.035, 1.02), 0.10, 0.085, jacket)
    limb(f"{word} sleeve cuff", (sign * 0.35, -0.035, 1.035), (sign * 0.35, -0.035, 0.99), 0.098, 0.096, jacket_dark)
    ellipsoid(f"{word} hand", (sign * 0.35, -0.04, 0.90), (0.058, 0.045, 0.115), skin, 8, 5)
    for finger in range(4):
        fx = sign * (0.31 + finger * 0.024)
        limb(f"{word} finger {finger}", (fx, -0.058, 0.86), (fx, -0.059, 0.79), 0.012, 0.009, skin, 6)
    limb(f"{word} thumb", (sign * 0.305, -0.077, 0.93), (sign * 0.29, -0.092, 0.86), 0.016, 0.011, skin, 6)
    box(f"{word} shoulder seam", (sign * 0.28, -0.09, 1.46), (0.12, 0.011, 0.01), jacket_dark)

box("waist hem", (0, 0, 0.975), (0.49, 0.255, 0.055), jacket_dark, 0.008)
for z in (1.19, 1.29, 1.39):
    ellipsoid(f"jacket button {z}", (-0.065, -0.205, z), (0.009, 0.008, 0.009), metal, 8, 4)

# Angular portrait details, kept deliberately sparse at game scale.
ellipsoid("head", (0, -0.01, 1.685), (0.135, 0.107, 0.17), skin, 12, 7)
box("jaw", (0, -0.035, 1.62), (0.20, 0.16, 0.08), skin, 0.025)
ellipsoid("nose", (0, -0.118, 1.685), (0.028, 0.045, 0.055), shadow_skin, 8, 4)
ellipsoid("nose highlight", (0, -0.154, 1.686), (0.021, 0.012, 0.037), skin, 8, 4)
box("mouth shadow", (0, -0.12, 1.598), (0.07, 0.006, 0.009), shadow_skin)
box("stubble chin", (0, -0.117, 1.575), (0.09, 0.007, 0.025), shadow_skin)
for sign, word in [(-1, "L"), (1, "R")]:
    ellipsoid(f"{word} ear", (sign * 0.136, -0.008, 1.68), (0.022, 0.028, 0.046), skin, 8, 4)
    box(f"{word} eye socket", (sign * 0.052, -0.106, 1.715), (0.062, 0.01, 0.026), shadow_skin)
    ellipsoid(f"{word} eye", (sign * 0.052, -0.116, 1.715), (0.019, 0.008, 0.010), eye, 8, 4)
    box(f"{word} brow", (sign * 0.053, -0.117, 1.75), (0.072, 0.013, 0.016), hair)
    box(f"{word} cheek stubble", (sign * 0.086, -0.101, 1.625), (0.075, 0.009, 0.028), shadow_skin)

ellipsoid("hair cap", (0, 0.008, 1.805), (0.143, 0.12, 0.085), hair, 12, 5)
for i in range(7):
    x = -0.11 + i * 0.036
    length = 0.025 + (i % 3) * 0.016
    limb(f"fringe {i}", (x, -0.106, 1.82), (x + 0.014, -0.128, 1.79 - length), 0.024, 0.006, hair, 5)
for sign, word in [(-1, "L"), (1, "R")]:
    ellipsoid(f"{word} side hair", (sign * 0.121, 0.018, 1.745), (0.045, 0.075, 0.089), hair, 8, 4)
ellipsoid("back hair", (0, 0.105, 1.75), (0.115, 0.04, 0.085), hair, 10, 4)

# Triplanar looking pixel noise needs UVs; smart projection keeps every part editable.
for obj in list(bpy.context.scene.objects):
    if obj.type != "MESH":
        continue
    bpy.ops.object.select_all(action="DESELECT")
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.uv.smart_project(island_margin=0.02)
    bpy.ops.object.mode_set(mode="OBJECT")

bpy.ops.object.select_all(action="DESELECT")
meshes = [obj for obj in bpy.context.scene.objects if obj.type == "MESH"]
for obj in meshes:
    obj.select_set(True)
bpy.context.view_layer.objects.active = meshes[0]

blend_path = ROOT / "daniel_holloway.blend"
bpy.ops.wm.save_as_mainfile(filepath=str(blend_path))
bpy.ops.wm.obj_export(
    filepath=str(ROOT / "daniel_holloway.obj"),
    export_selected_objects=True,
    export_materials=True,
    forward_axis="NEGATIVE_Z",
    up_axis="Y",
    path_mode="RELATIVE",
)

# A simple preview makes it easy to judge the silhouette without opening Blender.
for obj in meshes:
    obj.select_set(False)
bpy.ops.object.camera_add(location=(2.4, -3.8, 2.2))
camera = bpy.context.object
target = Vector((0, 0, 0.93))
camera.rotation_euler = (target - camera.location).to_track_quat("-Z", "Y").to_euler()
camera.data.type = "ORTHO"
camera.data.ortho_scale = 2.35
bpy.context.scene.camera = camera
bpy.ops.object.light_add(type="AREA", location=(-2, -3, 4))
bpy.context.object.data.energy = 450
bpy.context.object.data.shape = "DISK"
bpy.context.object.data.size = 3
bpy.context.scene.world.color = (0.22, 0.22, 0.22)
scene = bpy.context.scene
scene.render.engine = "BLENDER_EEVEE_NEXT"
scene.render.resolution_x = 768
scene.render.resolution_y = 768
scene.render.resolution_percentage = 100
scene.render.image_settings.file_format = "PNG"
scene.render.filepath = str(ROOT / "preview.png")
scene.render.film_transparent = True
bpy.ops.render.render(write_still=True)
print(f"Created {blend_path}")
