# 1.2

- Added bindings for CloseEditor, SaveEditor, RefreshEditor.
- RefreshEditor performs a full refresh of the custom editor (closes and re-runs the editor). This is mostly useful while developing the custom editor and making modifications to its dependencies.
- Added a button to the toolbar to reload the custom asset editor (useful when developing the editor tool, calls `CLAssetEditorUtility.Run()`).

# 1.1

- Fixed the layout issues
- Added the ability to save default layouts to share with others. If other people don't have any layout for a custom editor, the default layout will be used the first time the custom editor is opened.
- Layouts are reset if the TabIds are modified in a given `CLAssetEditorUtility`

# 1.0

- Initial version