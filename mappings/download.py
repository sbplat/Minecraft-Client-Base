from packaging import version
import requests
import zipfile
import io
import tkinter as tk
from tkinter import ttk, messagebox

# https://github.com/bspkrs/MCPMappingViewer/tree/master
JSON_VERSIONS_URL = "https://maven.minecraftforge.net/de/oceanlabs/mcp/versions.json"
BASE_MAPPING_URL = "https://maven.minecraftforge.net/de/oceanlabs/mcp/mcp_{channel}/{map_ver}-{mc_ver}/mcp_{channel}-{map_ver}-{mc_ver}.zip"
NEW_BASE_SRG_URL = "https://maven.minecraftforge.net/de/oceanlabs/mcp/mcp_config/{mc_ver}/mcp_config-{mc_ver}.zip"
OLD_BASE_SRG_URL = "https://maven.minecraftforge.net/de/oceanlabs/mcp/mcp/{mc_ver}/mcp-{mc_ver}-srg.zip"

def get_versions():
    r = requests.get(JSON_VERSIONS_URL).json()
    versions = []

    for mc_ver in r:
        for channel in r[mc_ver]:
            for ver in r[mc_ver][channel]:
                versions.append([mc_ver, channel, ver])

    return versions

def select_version():
    mc_versions = get_versions()
    mc_versions.sort(key=lambda x: version.parse(x[0]))

    root = tk.Tk()
    root.title("Select a version")
    root.geometry("250x150")

    selected = tk.StringVar()
    selected.set(mc_versions[0])

    combo_box = ttk.Combobox(root, state="readonly", values=mc_versions, textvariable=selected, width=100)
    button = ttk.Button(root, text="Select", command=root.destroy)

    combo_box.pack()
    button.pack()

    root.mainloop()

    return selected.get().split(" ")

def confirm_download(mc_ver, channel, ver):
    msg = f"Download mappings for {mc_ver} {channel} {ver}?"
    msg_box = messagebox.askquestion("Confirm download", msg)
    return msg_box == "yes"

def download_and_extract(zip_url, extract_path):
    r = requests.get(zip_url)
    z = zipfile.ZipFile(io.BytesIO(r.content))
    z.extractall(extract_path)

mc_version, channel, ver = select_version()
is_new = version.parse(mc_version) >= version.parse("1.13")
print(f"Selected version: {mc_version} {channel} {ver} (new: {is_new})")

map_url = BASE_MAPPING_URL.format(mc_ver=mc_version, channel=channel, map_ver=ver)
srg_url = (NEW_BASE_SRG_URL if is_new else OLD_BASE_SRG_URL).format(mc_ver=mc_version)

if not confirm_download(mc_version, channel, ver):
    print("Aborting...")
    exit(0)

print(f"Downloading mappings from {map_url}")
download_and_extract(map_url, ".")
print(f"Downloading SRG from {srg_url}")
download_and_extract(srg_url, ".")
print("Done!")
