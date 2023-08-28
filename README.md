The intended use case of this plugin is to build custom tab based asset editors using BP Editor Utility Widgets.

# How to use (Minimal Setup)

- Create a new `CLWidgetContainerAsset` asset (Editor Utilities category).
- Create a new `CLAssetEditorUtility` asset (Editor Utilities category).
- Go to the `CLAssetEditorUtility` class defaults details, and set the `CLWidgetContainerAsset` and the tab definitions (1 Editor Utility Widget per tab).
- Override the `Run` event and call the `OpenEditor()` function.
- Go to the Content Browser, to your `CLAssetEditorUtility` and run the script (Right Click->Run).
- This will open an empty editor the first time. Go to the `Window` menu and manually add the tabs into your editor, save so the layout is saved for next uses.
- Now you can go back to the `CLAssetEditorUtility` and script the communication between the tabs, like in the example. 
(You can do this at any time but note that the `GetUtilityWidget()` and the bindings won't work if the tabs are not present when the asset editor is open, so manually adding the tabs is needed the first time the editor is created).

# Features

- Exposes `CLAssetEditorUtility` which is a UEditorUtilityObject with the ability of building custom asset editors.
- Exposes a custom Asset `CLWidgetContainerAsset` (`Editor Utilities` category) that can contain the custom editors (`CLAssetEditorUtility` need to reference this container in its class defaults details).
- The Editor Utility Widgets are created as usual and live each in a tab. This forces a decoupled approach (unintendedly), which is why `CLAssetEditorUtility` exists as it can be used to orchestrate the communication between the tabs.

The plugin comes with a simple demo (Content folder):
![](https://raw.githubusercontent.com/cronofear-dev/CLAssetEditor/main/Resources/demo.gif)

# Roadmap

Most things will be added as I need them in my project. For now, the following is planned:

- Bindings to editor events (Save, Close Editor, Focus Tab, etc).
- Fix the layout issues (see Limitations and Known Issues).

# Limitations and Known Issues

- Due to needing to manually add the tabs the first time a new editor is created, and since the layouts are saved per user, this means that each user working on the project will need to manually add the tabs the first time they open the editor (I will investigate if I can fix it by allowing to programmatically add the layouts in BP).
- Removing tabs defined in `CLAssetEditorUtility` and re-running the script will cause the tabs removed to be persistent in the editor (If you close it, the next time you open the asset editor they will reappear). To work around this, first manually remove the tabs from the editor, then remove them from the `CLAssetEditorUtility` and re-run the script.
    - Note: If you accidentally removed a tab definition first in `CLAssetEditorUtility`, re-add it using the same id (the id will be the same as the persistent tab name), then follow the steps above.
- The both issues above combined may be annoying when working with other people, so for now I recommend using this plugin only for personal projects or if you are working alone.
    - An easy fix when removing tabs in a `CLAssetEditorUtility`, is to rename the `CLAssetEditorUtility` asset aftetwards. Layouts are saved per the mentioned asset's name. This will force each user in the project to haing to manually re-add the tabs, but at least there won't be any tab persistence issue.
- `CLWidgetContainerAsset` can only contain unique Editor Utility Widgets types for the tabs (If 2 copies of the same type are added, the plugin can only make bindings with the last instance of such type). This can be worked around by duplicating an Editor Utility Widget and using the copy instead.
