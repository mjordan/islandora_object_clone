# Islandora Object Clone

Copies an Islandora object and persists the copy much in the same manner that [Node Clone](https://www.drupal.org/project/node_clone) does for Drupal nodes. At the end of the cloning process, you have a new object that is pretty much identical to the object it was clone from. Differences are listed below.

## Introduction

Performs a shallow clone of the source object, that is, it only copies the object properties and datastreams. It does not generate new objects corresponding to any children the source object has.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Installation

Same as any Drupal module.

## Configuration

You can filter objects that can be cloned by content model at `admin/islandora/tools/object_clone`.

## Usage

Users with permission to ingest objects see a "Clone" tab for each object. Clicking on this tab reveals a form that asks for the label and namespace of the cloned object. Clicking on the "Clone object" button in this form creates a new object based on the source object. The new object differs from its source in the following ways:

* the user is asked to supply the following properties of the new object (form defaults are taken from the source):
  * label
  * state
  * namespace
* the new object's owner is the user who cloned it
* the user is also asked to select which collection(s) to add the cloned object to.

Apart from these differences, the object's content model and other RELS-EXT relationships (except for collection membership) are the same, as are all datastreams (other than RELS-EXT).

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Use cases, suggestions, and bug reports, as well as pull requests, are welcome. Please see CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
