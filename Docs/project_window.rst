'Project' window
================

Presentation
------------

Principle
~~~~~~~~~~~

The **'Project'** window allows to organize the set of useful data on three tabs `'Scene' tab`_, `'Calculation' tab`_ and `'Results' tab`_. It follows a natural procedure, for a classical acoustical study, which consists in:

1. Giving all information (geometry, material, sound sources, receivers...),
2. Gtarting the calculation (using a specific code),
3. Representing the results.

In the definition and the realisation of a calculation, **users should follow the natural procedure**:

1. Define all information/data in the tab `'Scene' tab`_
2. Choose and parametrize the calculation code in the tab `'Calculation' tab`_
3. Explore, process and represent all results in the tab `'Results' tab`_

Definitions
~~~~~~~~~~~

All data, in each tab, are organized in **trees**, each tree containing **folders** (eventually with subfolders), each folder with **elements**. Each element is defined by several parameters that are showed in the **‘Properties’** window. These elements can be data, check boxes, character chains, scrolling lists, trees…

.. warning::
	Data defined in the **‘Scene’** tree depend on the numerical code that will be used in the tab **‘Calculation’** (i.e. depending on the calculation code, it is not necessary to defined all elements. See the user guide/manual of the code for more information).

.. note::
	Using Python scripts, users can add folders and elements in trees. This can be useful for integrating new numerical codes or specific processes. See the scripting guide of I-Simpa for more information.

Common features
~~~~~~~~~~~~~~~

Collapse/Expand a tree
^^^^^^^^^^^^^^^^^^^^^^

A tree/branch can be collapsed (or expanded) by a:

-  click on the - (or the +) symbol in front of a folder in a tree;
-  double-click on the folder.

Copy/Paste function in a tree
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The copy/paste function is valid in many folders and elements of trees. This can used to duplicate elements and folders in tree, or to copy elements and folders in a text editor (the format of elements is ‘XML’). Used the right click of the pointer (**‘Copy’** and **‘Paste’** actions) to copy/paste elements.

Copy/Paste function in a spreadsheet
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All data concerning an element are descried in a spreadsheet in the **‘Properties’** window. All cells of the spreadsheet (for a given element) can be copy/paste to another spreadsheet for an equivalent element. Copy/Paste is also functional to export data to an external spreadsheet software.

Rename an element in a tree
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Excepted for predefined folders and elements, all new folders and elements created by users can be renamed, either when creating the element/folder, either after creation using the **‘Rename’** option (right click of the pointer on the element/folder to be renamed).

Remove an element from a tree
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Excepted for predefined folders and elements, all new folders and elements created by users can be removed, using the **'Delete'** action (right click of the pointer on the element/folder to be renamed).

Create charts from tabular data
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. include:: create_charts.rst

.. include:: tab_scene.rst

.. include:: tab_calculation.rst

.. include:: tab_results.rst
