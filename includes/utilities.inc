<?php

/**
 * @file
 * Utility functions used by the Islandora Object Clone module.
 */

/**
 * Ingests a new Islandora object based on the source object.
 *
 * We can't perform a dumb 'clone' of the object because the owner may be
 * different, and the createdDate will certainly be different. So, we will
 * need to copy the properties of the source object and ingest the new
 * object using them; we'll also need to iterate over the source object's
 * datastreams and ingest the new version of each of those as well.
 *
 * We do not copy RELS-EXT or RELS-INT since they are rebuilt using
 * relationships.
 *
 * @param array $form_state
 *   The $form_state from the submitted confirmation form.
 *
 * @return string
 *   The PID of the cloned object.
 */
function islandora_object_clone_clone_object($form_state) {
  $source_pid = $form_state['values']['islandora_object_clone_source_pid'];
  $confirmed_namespace = trim($form_state['values']['islandora_object_clone_confirm_namespace']);
  $confirmed_label = trim($form_state['values']['islandora_object_clone_confirm_label']);
  $confirmed_state = $form_state['values']['islandora_object_clone_confirm_state'];
  $confirmed_collections = $form_state['values']['islandora_object_clone_confirm_collections'];

  $source_object = islandora_object_load($source_pid);

  $tuque = islandora_get_tuque_connection();
  $repository = $tuque->repository;
  $obj = $repository->constructObject($confirmed_namespace);

  global $user;
  $obj->owner = $user->name;
  $obj->label = $confirmed_label;
  $obj->state = $confirmed_state;
  $obj->logMessage = "Cloned from " . $source_pid . '.';

  // Let's talk relationships. Copy them all (except for collection
  // relationships) from the source to the new object.
  foreach ($source_object->relationships->get() as $rel) {
    if ($rel['predicate']['value'] != 'isMemberOfCollection') {
      $obj->relationships->add(
        $rel['predicate']['namespace'],
        $rel['predicate']['value'],
        $rel['object']['value']
      );
    }
  }
  // Define collection relationships based on form input values.
  foreach ($confirmed_collections as $collection) {
    $obj->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection);
  }

  // Datastreams.
  $cloned_datastreams = islandora_object_clone_datastreams($source_object);
  foreach ($cloned_datastreams as $ds) {
    $obj->ingestDatastream($ds);
  }

  $new_object = $repository->ingestObject($obj);

  if (variable_get('islandora_object_clone_allow_sequenced', FALSE)) {
    if (!islandora_object_clone_is_sequence_free($new_object)) {
      drupal_set_message(t("The cloned object you have just created (@pid) has sequence relationships and should appear next to its source object within its parent object. You may want to reorder it.", array('@pid' => $new_object->id)), 'warning');
    }
  }

  return $new_object->id;
}


/**
 * Clones the source object's datastreams.
 *
 * Note: Does not clone datastream relationships.
 *
 * @param object $source_object
 *   The source object.
 *
 * @return array
 *   A list of all the cloned datastreams to add to the cloned object.
 */
function islandora_object_clone_datastreams($source_object) {
  $skip_these = array('RELS-EXT', 'RELS-INT');
  $cloned_datastreams = array();
  foreach ($source_object as $source_ds) {
    if (!in_array($source_ds->id, $skip_these)) {
      $ds = $source_object->constructDatastream($source_ds->id, $source_ds->controlGroup);

      $ds->label = $source_ds->label;
      $ds->setContentFromString($source_ds->content);
      $ds->mimetype = $source_ds->mimetype;
      $ds->checksumType = $source_ds->checksumType;
      $ds->versionable = $source_ds->versionable;
      $ds->state = $source_ds->state;
      $ds->format = $source_ds->format;
      if (isset($source_ds->url)) {
        $ds->url = $source_ds->url;
      }

      foreach ($source_ds->relationships->get() as $rel) {
        $ds->relationships->add(
          $rel['predicate']['namespace'],
          $rel['predicate']['value'],
          $rel['object']['value']
        );
      }

      // Allow other modules to modify the newly constructed  datastream.
      drupal_alter('islandora_object_clone_newds', $ds, $source_object);

      $cloned_datastreams[] = $ds;
    }
  }

  return $cloned_datastreams;
}

/**
 * Query the resource index to get a list of collections.
 *
 * @return array
 *   Associative array of pid => label pairs.
 */
function islandora_object_clone_get_collections() {
  // Query the rindex to get all collections.
  $tuque = islandora_get_tuque_connection();
  $ri_query = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
select $object $label from <#ri>
     where  { $object fedora-model:hasModel <info:fedora/islandora:collectionCModel> ;
      fedora-model:label $label . }
order by ($label)';
  $results = $tuque->repository->ri->sparqlQuery($ri_query, 'unlimited');
  $collections = array();
  foreach ($results as $member) {
    $pid = preg_replace('/info:fedora\//', '', $member['object']['value']);
    $label = $member['label']['value'];
    // We don't want the root collection.
    if ($pid != 'islandora:root') {
      $collections[$pid] = $label . ' (' . $pid . ')';
    }
  }
  return $collections;
}

/**
 * Get the first collection that an object belongs to.
 *
 * @param object $object
 *   The source object.
 * 
 * @return array
 *   Array of collection PIDs the object belongs to.
 */
function islandora_object_clone_get_default_collections($object) {
  $collections = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
  $default_collections = array();
  foreach ($collections as $collection) {
    $default_collections[] = $collection['object']['value'];
  }
  return $default_collections;
}

/**
 * Query the resource index to get a list of all content model objects.
 *
 * @return array
 *   Associative array of pid => label pairs.
 */
function islandora_object_clone_get_cmodels() {
  $skip_these = array(
    'fedora-system:FedoraObject-3.0',
    'fedora-system:ServiceDefinition-3.0',
    'fedora-system:ContentModel-3.0',
    'fedora-system:ServiceDeployment-3.0',
  );

  // Query the rindex to get all cmodels.
  $tuque = islandora_get_tuque_connection();
  $ri_query = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
select $object $label from <#ri>
     where { $object fedora-model:hasModel <info:fedora/fedora-system:ContentModel-3.0> ;
      fedora-model:label $label .}';
  $results = $tuque->repository->ri->sparqlQuery($ri_query, 'unlimited');
  $cmodels = array();
  foreach ($results as $member) {
    if (!in_array($member['object']['value'], $skip_these)) {
      $pid = preg_replace('/info:fedora\//', '', $member['object']['value']);
      $label = $member['label']['value'];
      $cmodels[$pid] = $label;
    }
  }
  return $cmodels;
}

/**
 * Determine if an object is free of isSequenceNumberOf* relationships.
 *
 * @param object $object
 *   The source object.
 * 
 * @return bool
 *   TRUE if the object has no sequence relationships, FALSE if it has any.
 */
function islandora_object_clone_is_sequence_free($object) {
  $has_sequences = TRUE;
  foreach ($object->relationships->get() as $rel) {
    if (strstr($rel['predicate']['value'], 'isSequenceNumberOf')) {
      $has_sequences = FALSE;
    }
  }

  return $has_sequences;
}
