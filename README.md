The intended use case of this plugin is to build custom tab based asset editors using BP Editor Utility Widgets, as seen in: https://youtu.be/wJqOn88cU7o?feature=shared&t=1707

# How to use (Minimal Setup)

1) Create a new `CLWidgetContainerAsset` asset (Editor Utilities category).
2) Create a new `CLAssetEditorUtility` asset (Editor Utilities category).
3) Go to the `CLAssetEditorUtility` class defaults details, and set the `CLWidgetContainerAsset` and the tab definitions (1 Editor Utility Widget per tab).
4) Override the `Run` event and call the `OpenEditor()` function.
5) Go back to your `CLAssetEditorUtility` and run the script (Right Click->Run).
6) This will open an empty editor the first time. Go to the `Window` menu and manually add the tabs into your editor and save, so the layout is saved for next uses.
7) Now you can go back to the `CLAssetEditorUtility` and script the communication between the tabs, like in the example. 
(You can do this at any time but note that the `GetUtilityWidget()` and the bindings won't work if the tabs are not present when the asset editor is open, so manually adding the tabs is needed the first time the editor is created).
8) Note that any change to the class default `TabDefinitions.TabId` parameter will reset the editor layout (Step 6 needs to be repeated if that's the case).
9) Optional. Call the `SaveDefaultLayout` method inside `CLAssetEditorUtility` once your tool is finished (disconnect the node afterwards). This will create a layout file in `Project/Config/Layouts` that can be shared through VCS so other people in the project can see a default layout when opening the editor for the first time.

# Features

- Exposes `CLAssetEditorUtility` which is a UEditorUtilityObject with the ability of building custom asset editors.
- Exposes a custom Asset `CLWidgetContainerAsset` (`Editor Utilities` category) that can contain the custom editors (`CLAssetEditorUtility` need to reference this container in its class defaults details).
- The Editor Utility Widgets are created as usual and live each in a tab. This forces a decoupled approach (unintendedly), which is why `CLAssetEditorUtility` exists as it can be used to orchestrate the communication between the tabs.

The plugin comes with a simple demo (Content folder):
![](https://raw.githubusercontent.com/cronofear-dev/CLAssetEditor/main/Resources/demo.gif)

# Roadmap

Things will be added as I need them in my project. For now, the following is planned:

- Bindings to editor events (Save, Close Editor, Focus Tab, etc).
- Add a button to the toolbar to reload the custom asset editor (useful when developing the editor tool, calls `CLAssetEditorUtility.Run()`).
- Add a button to the toolbar to save the default layout (calls `CLAssetEditorUtility.SaveDefaultLayout()`)

# Limitations and Known Issues

- `CLWidgetContainerAsset` can only contain unique Editor Utility Widgets types for the tabs (If 2 copies of the same type are added, the plugin can only make bindings with the last instance of such type). This can be worked around by duplicating an Editor Utility Widget class and using the copy instead.
