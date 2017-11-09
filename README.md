# Islandora Object Clone

Copies an Islandora object and persists the copy much in the same manner that [Node Clone](https://www.drupal.org/project/node_clone) does for Drupal nodes.

## Introduction

Performs a shallow clone of the source object, that is, it only copies the object properties and datastreams. It does not generate new objects corresponding to any children the source object has.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Installation

Same as any Drupal module.

## Usage

Users with permission to ingest objects see a "Clone" tab for each object. Clicking on this tab reveals a form that asks for the label and namespace of the cloned object. Clicking on the "Clone object" button in this form creates a new object based on the source object, with the current user as its owner. 

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Use cases, suggestions, and bug reports, as well as pull requests, are welcome. Please see CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
