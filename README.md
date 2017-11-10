# Islandora Object Clone

Copies an Islandora object and persists the copy much in the same manner that [Node Clone](https://www.drupal.org/project/node_clone) does for Drupal nodes. At the end of the cloning process (click a tab, optionally fill in a couple of form fields), you have a new object that is pretty much identical to the object it was cloned from. Differences are listed below.

## Introduction

Performs a shallow clone of the source object, that is, it only copies the object properties and datastreams. It does not generate new objects corresponding to any children the source object has.

MODS and DC are copied as is, with the exception that the new object's PID is added to a dc:identifier element. The source object's PID that existed in its DC is therefore also present in the new DC datastream. Also, any identifiers or other source-object-specific data, like DOIs, in MODS or other datastreams is also carried over. Reviewing the cloned metadata for this sort of data is probably a good habit to get into.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Installation

Same as any Drupal module.

## Configuration

You will need to enable which content models source objects must have at `admin/islandora/tools/object_clone`.

There is an option to allow objects that have sequence relationships ('isSequenceNumber' and 'isSequenceNumberOfxxx') to be cloneable. These include all paged content objects and any other objects that are children of compound objects. If checked, these objects will be cloneable like other objects, and users who clone them will be reminded to review their order within their parent objects. If unchecked, these objects will not be cloneable.

## Usage

Users with permission to manage objects (as defined by the Islandora module) are provided access to a "Clone" tab for each object. Clicking on this tab reveals a form that asks for the label, state, namespace, and collections of the object to be created. Clicking on the "Clone object" button in this form creates a new object based on the source object and ingests it. The new object differs from its source in the following ways:

* the new object's owner is the user who cloned it
* the user is asked to supply the following properties of the new object (form defaults are taken from the source object):
  * label
  * state (is overridden by Islandora Simple Workflow)
  * namespace
* the user is also asked to select which collection(s) to add the cloned object to (form defaults to the source object's immediate parent collection(s)).

Apart from these differences, the new object's content model and other RELS-EXT relationships (except for collection membership) are the same as the source object, as are all datastreams other than RELS-EXT, which is rebuilt in the new object. RELS-INT is also rebuilt to incorporate relationships associated with datastreams.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Use cases, suggestions, and bug reports, as well as pull requests, are welcome. Please see CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
